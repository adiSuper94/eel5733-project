#include "bank.h"
#include "ringbuffer.h"
#include <semaphore.h>
#include <stdio.h>
#include <string.h>

struct Bank *initBank() {
  struct Bank *bank = (struct Bank *)calloc(sizeof(struct Bank), 1);
  bank->capacity = 32;
  bank->size = 0;
  bank->tellers = 2;
  bank->accounts = calloc(sizeof(struct Account), bank->capacity);
  bank->transactionCount = 0;
  if (sem_init(&bank->sem, 0, 1) == -1) {
    exit(0);
  }
  return bank;
}

struct Account *getAccountByAccountNumber(struct Bank *bank, int accountNumber) {
  for (int i = 0; i < bank->size; i++) {
    if (bank->accounts[i].accountNumber == accountNumber) {
      return &bank->accounts[i];
    }
  }
  return NULL;
}

void addAccount(struct Bank *bank, int accountNumber, int initBalance) {
  struct Account *account = (struct Account *)calloc(sizeof(struct Account), 1);
  account->balance = initBalance;
  account->accountNumber = accountNumber;
  account->activeTransaction = false;
  account->accountId = bank->size++;
  if (sem_init(&account->sem, 0, 1) == -1) {
    exit(0);
  }
  sem_wait(&(bank->sem));
  while (bank->capacity <= account->accountId) {
    resize(bank);
  }
  bank->accounts[account->accountId] = *account;
  sem_post(&(bank->sem));
}

void resize(struct Bank *bank) {
  struct Account *newAccounts =
      (struct Account *)realloc(bank->accounts, sizeof(struct Account) * bank->capacity * 2);
  if (newAccounts == NULL) {
    exit(1);
  }
  bank->capacity *= 2;
  bank->accounts = newAccounts;
  return;
}

void transaction(struct Bank *bank, int from, int to, int value) {
  sem_wait(&(bank->sem)); // acquire bank lock;
  struct Account *toAccount = getAccountByAccountNumber(bank, to);
  struct Account *fromAccount = getAccountByAccountNumber(bank, from);
  while (fromAccount->activeTransaction || toAccount->activeTransaction) {
    if (fromAccount->activeTransaction) {
      sem_post(&(bank->sem)); // release bank lock
      sem_wait(&(fromAccount->sem));
      sem_post(&(fromAccount->sem));
      sem_wait(&(bank->sem)); // acquire bank lock;
    }
    if (toAccount->activeTransaction) {
      sem_post(&(bank->sem)); // release bank lock
      sem_wait(&(toAccount->sem));
      sem_post(&(toAccount->sem));
      sem_wait(&(bank->sem)); // acquire bank lock;
    }
  }
  sem_wait(&(toAccount->sem));
  sem_wait(&(fromAccount->sem));
  fromAccount->activeTransaction = true;
  toAccount->activeTransaction = true;
  sem_post(&(bank->sem)); // release bank lock
  toAccount->balance += value;
  fromAccount->balance -= value;
  sem_wait(&(bank->sem)); // acquire bank lock;
  fromAccount->activeTransaction = false;
  toAccount->activeTransaction = false;
  sem_post(&fromAccount->sem);
  sem_post(&toAccount->sem);
  sem_post(&(bank->sem)); // release bank lock
}

void tellerInit(void *arg) {
  struct TellerThreadArg *tellerThreadArg = (struct TellerThreadArg *)arg;
  struct RingBuffer *rb = tellerThreadArg->rb;
  struct Bank *bank = tellerThreadArg->bank;
  while (true) {
    char *transactionString = removeFromRingBuff(rb);
    if (strcmp(transactionString, "END") == 0) {
      break;
    }
    if (transactionString[0] == 'T') {
      int to, from, value;
      sscanf(transactionString, "Transfer %d %d %d", &from, &to, &value);
      // printf("%d, %d, %d \n", from, to, value);
      transaction(bank, from, to, value);
    } else {
      int accountNumber, initBalance;
      sscanf(transactionString, "%d %d", &accountNumber, &initBalance);
      addAccount(bank, accountNumber, initBalance);
    }
    free(transactionString);
  }
}

void printResult(struct Bank *bank) {
  for (int i = 0; i < bank->size; i++) {
    printf("%d %d\n", bank->accounts[i].accountNumber, bank->accounts[i].balance);
  }
}
