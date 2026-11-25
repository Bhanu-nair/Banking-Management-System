#include "banking.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Customer *g_customers=NULL;
void show_menu(void){print_header("C BANKING MANAGEMENT SYSTEM");puts("1. Create New Account\n2. Update Account Details\n3. Delete Account\n4. View Account\n5. Deposit Money\n6. Withdraw Money\n7. View All Accounts\n8. Transaction History\n9. Save Data\n0. Exit");puts("------------------------------------------------------------");}
int main(void){srand((unsigned int)time(NULL));if(!load_data())puts("The application will continue with the records that could be recovered (if any).");else puts("Bank data loaded.");int choice;for(;;){show_menu();choice=input_int("Enter your choice: ",0,9);switch(choice){case 1:create_account();break;case 2:update_account();break;case 3:delete_account();break;case 4:view_account();break;case 5:deposit_money();break;case 6:withdraw_money();break;case 7:list_accounts();break;case 8:show_transaction_history();break;case 9:save_data();break;case 0:save_data();free_all_customers();puts("Thank you for using the Banking Management System.");return 0;}if(choice!=0)pause_enter();}}
