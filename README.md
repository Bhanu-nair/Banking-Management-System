# C Banking Management System

A modular, menu-driven Banking Management System developed in **C (C11)** for educational purposes. The application allows a bank operator to create and manage customer accounts, authenticate account holders with account numbers and PINs, perform deposits and withdrawals, inspect transaction histories, and save records between program runs.

> **Educational project only:** This is a console-based simulation. It is not suitable for real banking or for storing real customer information. PIN storage and the binary file format are not production-grade security mechanisms.

## Features

- Create customer accounts with generated unique account numbers and four-digit PINs.
- Set a fixed opening balance of **Rs. 1,000.00** and record it as the first transaction.
- Support Savings and Current account types.
- Authenticate protected operations using account number and PIN.
- Update name, address, phone, email, date of birth, and account type.
- Delete an account after explicit confirmation.
- View one account with the PIN hidden, or list all accounts.
- Deposit and withdraw money with positive-amount and sufficient-balance checks.
- Maintain a per-account transaction history with transaction ID, type, amount, resulting balance, and timestamp.
- Save and reload customer and transaction records using a binary data file.
- Validate user input and handle empty account lists and common invalid entries.
- Build the multi-file project with a Makefile and strict compiler warnings.

## Prerequisites

- GCC or another C11-compatible compiler
- GNU Make
- Linux, macOS, or Windows with WSL (the supplied Makefile uses Unix-style commands)

No third-party libraries are required.

## Build and run

```bash
make
make run
```

Or run the executable directly:

```bash
./banking
```

Remove compiled output without deleting saved banking data:

```bash
make clean
```

## Main menu

| Option | Operation |
|---:|---|
| 1 | Create New Account |
| 2 | Update Account Details |
| 3 | Delete Account |
| 4 | View Account |
| 5 | Deposit Money |
| 6 | Withdraw Money |
| 7 | View All Accounts |
| 8 | Transaction History |
| 9 | Save Data |
| 0 | Exit (saves before closing) |

### Account creation

The operator enters the customer's name, address, phone number, email, date of birth, and account type. The system generates an account number and PIN. The PIN is displayed at creation time only; the account display does not reveal it. Every new account starts with Rs. 1,000.00.

### Deposits and withdrawals

The operator must authenticate using the account number and PIN. Amounts must be positive and within the configured maximum. A withdrawal greater than the available balance is rejected. Successful balance changes are added to the customer's transaction history.

## Project structure

```text
c-banking-management-system/
├── banking.h       # Shared structures, enums, constants and prototypes
├── main.c          # Program entry point and main menu
├── input.c         # Safe input, validation and display helpers
├── accounts.c      # Account creation, authentication, update, delete and display
├── transactions.c  # Deposits, withdrawals and transaction history
├── storage.c       # Binary file persistence
├── Makefile        # Build, run and clean targets
├── .gitignore      # Excludes binaries, build output and local data
└── README.md
```

## Data model

### Customer

Each customer record stores an account number, PIN, name, address, phone, email, account type, current balance, date of birth, account opening timestamp, a pointer to the customer's transaction list, and a pointer to the next customer.

### Transaction

Each transaction stores a transaction ID, transaction type (opening, deposit, or withdrawal), amount, balance after the transaction, timestamp, and a pointer to the next transaction.

### Linked-list design

All customers are stored in one singly linked list. Each customer owns a separate singly linked list of transactions. This is a **nested linked-list** design and uses dynamic memory allocation.

## Data storage

- Runtime data is held in dynamically allocated linked lists.
- `bank_data.dat` stores customer records and their transaction records in binary form.
- Saving writes to `bank_data.tmp` first and replaces the main data file only after successful writes.
- Data is loaded when the application starts and saved on explicit Save and Exit.
- The data file is excluded from Git by `.gitignore`.

**Portability limitation:** The current implementation writes C structures in binary form. Such files can depend on compiler, architecture, structure padding, and endianness. Use the same build environment for reliable reuse. A portable serialization format or database would be preferable for a larger system.

## Concepts demonstrated

- C structures and enumerations
- Header files and modular programming
- Functions and pointers
- Singly linked lists and nested linked lists
- Dynamic memory allocation (`calloc` and `free`)
- Input parsing and validation
- Date/time handling using `<time.h>`
- File handling in binary mode
- Build automation using Make
- Basic testing and debugging practices

## Validation and limitations

The program checks required text fields, basic phone/email formats, date-of-birth validity, numeric menu choices, positive transaction amounts, and sufficient funds for withdrawals. It uses a simple generated PIN and stores it directly in the local binary file. Random account/PIN generation and authentication are for demonstration only; they are not cryptographically secure. This project does not connect to a bank, payment gateway, network, or real customer database.

## Suggested future enhancements

- Hash PINs with a suitable password-hashing library and add retry lockout.
- Use operating-system cryptographic randomness for account credentials.
- Store money as integer paise/cents to avoid floating-point rounding issues.
- Add account-to-account transfers with atomic debit/credit handling.
- Add savings interest and current-account minimum-balance rules.
- Export account statements to CSV or PDF.
- Replace raw-structure storage with a portable format or SQLite database.
- Add automated unit and integration tests.
- Add administrator/customer roles and a graphical or web interface.

## Testing checklist

- [ ] Create multiple accounts and verify unique account numbers.
- [ ] Verify the opening balance and opening transaction.
- [ ] Test correct and incorrect account credentials.
- [ ] Update each customer field and confirm it remains after restart.
- [ ] Cancel and confirm account deletion.
- [ ] Deposit a valid amount and inspect the new balance/history.
- [ ] Attempt a withdrawal above the balance and verify no balance change.
- [ ] Withdraw a valid amount and inspect the history.
- [ ] Save, close, reopen, and verify account and transaction data.
- [ ] Test empty input, invalid menu choices, and invalid numeric input.
- [ ] Run `make clean && make` and check for compiler warnings.

## Author

**Your Name**  
C Programming Project | Banking Management System

Replace this placeholder with your name and profile links before publishing the repository.

## License

This project is intended for educational use. Add a license file of your choice before publishing or redistributing it.
