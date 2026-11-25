#include "banking.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

static unsigned long long next_account_number(void) {
    unsigned long long candidate;
    do { candidate = 1000000000ULL + (unsigned long long)rand() % 9000000000ULL; } while (find_customer(candidate));
    return candidate;
}
static unsigned int make_pin(void) { return 1000U + (unsigned int)(rand() % 9000); }
static int valid_phone(const char *s) { size_t n = strlen(s); if (n < 10 || n > 15) return 0; for (size_t i=0;i<n;i++) if (!isdigit((unsigned char)s[i]) && !(i==0 && s[i]=='+')) return 0; return 1; }
static int valid_email(const char *s) { const char *at = strchr(s,'@'); return at && at != s && strchr(at,'.') && strchr(at,'.') < s + strlen(s) - 1; }
static int valid_date(const char *s) { int y,m,d; char tail; if (sscanf(s,"%4d-%2d-%2d%c",&y,&m,&d,&tail)!=3 || strlen(s)!=10) return 0; struct tm t={0}; t.tm_year=y-1900; t.tm_mon=m-1; t.tm_mday=d; t.tm_isdst=-1; time_t v=mktime(&t); if(v==(time_t)-1) return 0; struct tm *r=localtime(&v); if(!r || r->tm_year!=y-1900 || r->tm_mon!=m-1 || r->tm_mday!=d || v>time(NULL)) return 0; return 1; }

Customer *find_customer(unsigned long long account_no) { for(Customer *p=g_customers;p;p=p->next) if(p->account_no==account_no) return p; return NULL; }
Customer *authenticate(void) { char s[40]; unsigned long long no; unsigned int pin; input_line("Account number: ",s,sizeof s,0); char *e; no=strtoull(s,&e,10); if(*e) no=0; int p=input_int("4-digit PIN: ",0,9999); pin=(unsigned int)p; Customer *c=find_customer(no); if(!c || c->pin!=pin) { puts("Invalid account number or PIN."); return NULL; } return c; }

static int append_transaction(Customer *c, int type, double amount) {
    Transaction *t=calloc(1,sizeof *t); if(!t) return 0;
    t->id=((unsigned long long)time(NULL)*1000ULL)+(unsigned long long)(rand()%1000);
    t->type=type; t->amount=amount; t->balance_after=c->balance; t->timestamp=time(NULL);
    if(!c->transactions) c->transactions=t; else { Transaction *p=c->transactions; while(p->next)p=p->next; p->next=t; }
    return 1;
}

void create_account(void) {
    Customer *c=calloc(1,sizeof *c); if(!c){ puts("Memory allocation failed."); return; }
    print_header("CREATE NEW ACCOUNT");
    c->account_no=next_account_number(); c->pin=make_pin();
    input_line("Customer name: ",c->name,sizeof c->name,0);
    input_line("Address: ",c->address,sizeof c->address,0);
    do { input_line("Phone (10-15 digits, optional leading +): ",c->phone,sizeof c->phone,0); if(!valid_phone(c->phone)) puts("Invalid phone number."); } while(!valid_phone(c->phone));
    do { input_line("Email: ",c->email,sizeof c->email,0); if(!valid_email(c->email)) puts("Invalid email address."); } while(!valid_email(c->email));
    do { input_line("Date of birth (YYYY-MM-DD): ",c->dob,sizeof c->dob,0); if(!valid_date(c->dob)) puts("Enter a valid date not in the future."); } while(!valid_date(c->dob));
    c->account_type=input_int("Account type (1-Savings, 2-Current): ",1,2);
    c->balance=OPENING_BALANCE; c->opened_at=time(NULL); if(!append_transaction(c,OPENING,OPENING_BALANCE)){free(c);puts("Could not create opening transaction. Account was not created.");return;}
    if(!g_customers) g_customers=c; else { Customer *p=g_customers; while(p->next)p=p->next; p->next=c; }
    puts("\nAccount created successfully!"); printf("Account number : %llu\nTemporary PIN  : %04u\nOpening balance: Rs. %.2f\n",c->account_no,c->pin,c->balance); puts("Please keep your PIN safe. It will not be displayed again.");
}

void update_account(void) {
    print_header("UPDATE ACCOUNT DETAILS"); Customer *c=authenticate(); if(!c)return;
    int choice;
    do { puts("\n1. Name\n2. Address\n3. Phone\n4. Email\n5. Date of birth\n6. Account type\n0. Back"); choice=input_int("Select field: ",0,6);
        switch(choice) {
            case 1: input_line("New name: ",c->name,sizeof c->name,0); break;
            case 2: input_line("New address: ",c->address,sizeof c->address,0); break;
            case 3: do { input_line("New phone: ",c->phone,sizeof c->phone,0); } while(!valid_phone(c->phone)); break;
            case 4: do { input_line("New email: ",c->email,sizeof c->email,0); } while(!valid_email(c->email)); break;
            case 5: do { input_line("New DOB (YYYY-MM-DD): ",c->dob,sizeof c->dob,0); } while(!valid_date(c->dob)); break;
            case 6: c->account_type=input_int("1-Savings, 2-Current: ",1,2); break;
            default: break;
        }
        if(choice) puts("Account details updated.");
    } while(choice!=0);
}

void delete_account(void) {
    print_header("DELETE ACCOUNT"); Customer *auth=authenticate(); if(!auth)return;
    if(!input_yes_no("Are you sure you want to permanently delete this account? (Y/N): ")) { puts("Deletion cancelled."); return; }
    Customer **link=&g_customers; while(*link && *link!=auth) link=&(*link)->next;
    if(*link) *link=auth->next;
    Transaction *t=auth->transactions; while(t){Transaction *n=t->next; free(t); t=n;} free(auth);
    puts("Account deleted successfully.");
}

static void display_customer(const Customer *c) {
    char opened[32]; format_time(c->opened_at,opened,sizeof opened);
    printf("\nAccount number : %llu\nName           : %s\nAddress        : %s\nPhone          : %s\nEmail          : %s\nAccount type   : %s\nBalance        : Rs. %.2f\nDate of birth  : %s\nOpened on      : %s\n",c->account_no,c->name,c->address,c->phone,c->email,c->account_type==SAVINGS?"Savings":"Current",c->balance,c->dob,opened);
}
void view_account(void) { print_header("VIEW ACCOUNT"); Customer *c=authenticate(); if(c)display_customer(c); }
void list_accounts(void) { print_header("ALL BANK ACCOUNTS"); if(!g_customers){puts("No accounts found.");return;} printf("%-14s %-22s %-10s %-14s\n","ACCOUNT NO","CUSTOMER","TYPE","BALANCE"); puts("----------------------------------------------------------------"); int count=0; for(Customer *c=g_customers;c;c=c->next){printf("%-14llu %-22.22s %-10s Rs.%10.2f\n",c->account_no,c->name,c->account_type==SAVINGS?"Savings":"Current",c->balance);count++;} printf("\nTotal accounts: %d\n",count); }
void free_all_customers(void) { Customer *c=g_customers; while(c){Customer *next=c->next; Transaction *t=c->transactions; while(t){Transaction *tn=t->next;free(t);t=tn;}free(c);c=next;}g_customers=NULL; }
