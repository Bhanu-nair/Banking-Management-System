#include "banking.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int record_transaction(Customer *c,int type,double amount) {
    Transaction *t=calloc(1,sizeof *t); if(!t)return 0;
    t->id=((unsigned long long)time(NULL)*1000ULL)+(unsigned long long)(rand()%1000);
    t->type=type;t->amount=amount;t->balance_after=c->balance;t->timestamp=time(NULL);
    if(!c->transactions)c->transactions=t;else{Transaction*p=c->transactions;while(p->next)p=p->next;p->next=t;}return 1;
}
static void perform_transaction(int type) {
    print_header(type==DEPOSIT?"DEPOSIT MONEY":"WITHDRAW MONEY"); Customer *c=authenticate(); if(!c)return;
    double amount=input_money("Enter amount (Rs.): ");
    if(type==WITHDRAWAL && amount>c->balance){printf("Insufficient funds. Available balance: Rs. %.2f\n",c->balance);return;}
    double old=c->balance; c->balance += type==DEPOSIT?amount:-amount;
    if(!record_transaction(c,type,amount)){c->balance=old;puts("Transaction could not be recorded. No balance change was made.");return;}
    printf("%s successful.\nNew balance: Rs. %.2f\n",type==DEPOSIT?"Deposit":"Withdrawal",c->balance);
}
void deposit_money(void){perform_transaction(DEPOSIT);} void withdraw_money(void){perform_transaction(WITHDRAWAL);}
void show_transaction_history(void) {
    print_header("TRANSACTION HISTORY"); Customer *c=authenticate(); if(!c)return;
    if(!c->transactions){puts("No transactions found.");return;}
    printf("Account: %llu | Customer: %s\n",c->account_no,c->name);
    printf("%-14s %-13s %-14s %-16s %-20s\n","TXN ID","TYPE","AMOUNT","BALANCE AFTER","DATE/TIME");puts("--------------------------------------------------------------------------------");
    for(Transaction*t=c->transactions;t;t=t->next){char dt[32];format_time(t->timestamp,dt,sizeof dt);const char*type=t->type==OPENING?"Opening":t->type==DEPOSIT?"Deposit":"Withdrawal";printf("%-14llu %-13s Rs.%10.2f Rs.%12.2f %-20s\n",t->id,type,t->amount,t->balance_after,dt);}
}
