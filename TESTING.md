# Test Plan

| ID | Test | Expected result |
|---|---|---|
| T01 | Enter a menu choice outside 0-9 | Input is rejected and requested again |
| T02 | Create account with valid fields | Account number, PIN and opening balance are displayed |
| T03 | Enter invalid phone/email/date | Input is requested again |
| T04 | View account with correct credentials | Account details appear; PIN remains hidden |
| T05 | Authenticate with wrong account/PIN | Generic authentication failure |
| T06 | Deposit a positive amount | Balance increases and history is updated |
| T07 | Withdraw more than balance | Transaction is rejected; balance is unchanged |
| T08 | Withdraw a valid amount | Balance decreases and history is updated |
| T09 | View transaction history | Opening and successful transactions appear in order |
| T10 | Update account details | Selected field is updated |
| T11 | Cancel account deletion | Account remains in the list |
| T12 | Confirm account deletion | Account and its in-memory history are removed |
| T13 | Save, exit and restart | Saved records are loaded |
| T14 | Start with no data file | Application starts with an empty account list |

Record actual outcomes and date when executing the tests. Do not mark a test as passed until it has been run.
