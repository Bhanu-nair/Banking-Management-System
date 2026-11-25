#ifndef BANKING_H
#define BANKING_H
#include <stddef.h>
#include <time.h>

#define NAME_LEN 60
#define ADDRESS_LEN 120
#define PHONE_LEN 16
#define EMAIL_LEN 80
#define DATE_LEN 11
#define DATA_FILE "bank_data.dat"
#define TEMP_FILE "bank_data.tmp"
#define OPENING_BALANCE 1000.00
#define MAX_AMOUNT 100000000.00

typedef enum { SAVINGS = 1, CURRENT = 2 } AccountType;
typedef enum { DEPOSIT = 1, WITHDRAWAL = 2, OPENING = 3 } TransactionType;

typedef struct Transaction {
    unsigned long long id;
    int type;
    double amount;
    double balance_after;
    time_t timestamp;
    struct Transaction *next;
} Transaction;

typedef struct Customer {
    unsigned long long account_no;
    unsigned int pin;
    char name[NAME_LEN];
    char address[ADDRESS_LEN];
    char phone[PHONE_LEN];
    char email[EMAIL_LEN];
    int account_type;
    double balance;
    char dob[DATE_LEN];
    time_t opened_at;
    Transaction *transactions;
    struct Customer *next;
} Customer;

extern Customer *g_customers;

void input_line(const char *prompt, char *out, size_t size, int allow_empty);
int input_int(const char *prompt, int min, int max);
double input_money(const char *prompt);
int input_yes_no(const char *prompt);
void pause_enter(void);
void print_header(const char *title);
void format_time(time_t value, char *out, size_t size);

Customer *find_customer(unsigned long long account_no);
Customer *authenticate(void);
void create_account(void);
void update_account(void);
void delete_account(void);
void view_account(void);
void list_accounts(void);
void free_all_customers(void);
void deposit_money(void);
void withdraw_money(void);
void show_transaction_history(void);
int save_data(void);
int load_data(void);
void show_menu(void);

#endif
