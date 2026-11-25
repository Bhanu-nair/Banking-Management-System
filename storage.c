#include "banking.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define FILE_MAGIC "CBANK01"
typedef struct { char magic[8]; uint32_t version; uint32_t customer_count; } FileHeader;
typedef struct { Customer data; uint32_t transaction_count; } DiskCustomer;

int save_data(void) {
    FILE *fp=fopen(TEMP_FILE,"wb"); if(!fp){perror("Cannot create temporary data file");return 0;}
    FileHeader h={{0},1,0}; memcpy(h.magic,FILE_MAGIC,sizeof FILE_MAGIC); for(Customer*c=g_customers;c;c=c->next)h.customer_count++;
    int ok=fwrite(&h,sizeof h,1,fp)==1;
    for(Customer*c=g_customers;c && ok;c=c->next){DiskCustomer dc; memset(&dc,0,sizeof dc); dc.data=*c; dc.data.next=NULL;dc.data.transactions=NULL;for(Transaction*t=c->transactions;t;t=t->next)dc.transaction_count++; if(fwrite(&dc,sizeof dc,1,fp)!=1){ok=0;break;} for(Transaction*t=c->transactions;t;t=t->next){Transaction dt=*t;dt.next=NULL;if(fwrite(&dt,sizeof dt,1,fp)!=1){ok=0;break;}}}
    if (fflush(fp) != 0) { ok = 0; }
    if (fclose(fp) != 0) { ok = 0; }
    if(!ok){remove(TEMP_FILE);puts("Save failed. Existing data file was not replaced.");return 0;}
    if(rename(TEMP_FILE,DATA_FILE)!=0){perror("Could not replace data file");remove(TEMP_FILE);return 0;}
    printf("Saved %u account(s) successfully.\n",h.customer_count);return 1;
}

int load_data(void) {
    FILE *fp=fopen(DATA_FILE,"rb"); if(!fp)return 1;
    FileHeader h; if(fread(&h,sizeof h,1,fp)!=1 || memcmp(h.magic,FILE_MAGIC,sizeof FILE_MAGIC)!=0 || h.version!=1 || h.customer_count>100000){puts("Warning: data file is invalid. Starting with an empty system.");fclose(fp);return 0;}
    Customer *head=NULL,**tail=&head; int valid=1;
    for(uint32_t i=0;i<h.customer_count && valid;i++){
        DiskCustomer dc; if(fread(&dc,sizeof dc,1,fp)!=1 || dc.transaction_count>100000){valid=0;break;}
        Customer *c=calloc(1,sizeof *c);if(!c){valid=0;break;} *c=dc.data;c->next=NULL;c->transactions=NULL;
        if(c->account_type!=SAVINGS && c->account_type!=CURRENT)valid=0;
        if(!memchr(c->name,'\0',sizeof c->name)||!memchr(c->address,'\0',sizeof c->address)||!memchr(c->phone,'\0',sizeof c->phone)||!memchr(c->email,'\0',sizeof c->email)||c->balance<0.0)valid=0;
        Transaction **tt=&c->transactions;
        for(uint32_t j=0;j<dc.transaction_count && valid;j++){Transaction dt;if(fread(&dt,sizeof dt,1,fp)!=1){valid=0;break;}Transaction*t=calloc(1,sizeof *t);if(!t){valid=0;break;}*t=dt;t->next=NULL;if(t->type<DEPOSIT||t->type>OPENING||t->amount<0.0){free(t);valid=0;break;}*tt=t;tt=&t->next;}
        if(!valid){Transaction*t=c->transactions;while(t){Transaction*n=t->next;free(t);t=n;}free(c);break;}
        *tail=c;tail=&c->next;
    }
    if(valid && fgetc(fp)!=EOF)valid=0;
    fclose(fp);
    if(!valid){Customer*c=head;while(c){Customer*n=c->next;Transaction*t=c->transactions;while(t){Transaction*tn=t->next;free(t);t=tn;}free(c);c=n;}puts("Warning: data file is damaged. No partial records were loaded.");return 0;}
    g_customers=head;return 1;
}
