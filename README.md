[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Unv3Nbtc)
[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-2e0aaae1b6195c2367325f4f02e2d04e9abb55f0b24a779b69b11b9e10269abc.svg)](https://classroom.github.com/online_ide?assignment_repo_id=20633330&assignment_repo_type=AssignmentRepo)
# Student Project Template 
## Build & Run
## Release workflow
- We keep ONE repository for the whole project.
- At the end of each checkpoint, mark a release tag and add your report.

### Mark a release tag
###  end of Release 1:
git tag release-1
git push origin release-1

###  end of Release 2:
git tag release-2
git push origin release-2

### Folders for documents
docs/release-1/  # slides, SRS/SDP updates, test report

docs/release-2/

docs/release-3/

# Currency Exchange System – Fundamentals of C++ Project

## Overview
This project implements a **Currency Exchange System** as part of the TalTech course  
**Fundamentals of C++ Programming**.  
The system automates currency conversions in a small exchange office.  

## Team
- Magnus Jaaska (Developer, Documentation)  

## Features
- Perform currency exchange (with manual rates).
- Validate reserves before confirming transactions.
- Print receipts for clients.
- Generate end-of-day reports.
- Notify low reserves.

## Repository Structure
- `src/` — C++ source files (Cashier, Transaction, Receipt, Report, CurrencyReserve).
- `tests/` — Small test programs for validation.
- `docs/release-1/` — SRS, SDS, CRC cards, UML diagrams, slides, traceability.
- `Makefile` — Build and run commands.
- `README.md` — This file.
- `.gitignore` — Ignore build artifacts and IDE configs.

## Build Instructions
### Linux / macOS
```bash
make
make run
