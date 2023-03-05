#pragma once
#include "./ringbuffer.h"
#include <semaphore.h>
#include <stdbool.h>

struct Account {
  int accountNumber;
  int balance;
  sem_t sem;
  bool activeTransaction;
  int accountId;
};

struct Bank {
  struct Account *accounts;
  int capacity;
  int size;
  int tellers;
  int transactionCount;
  sem_t sem;
};

struct TellerThreadArg {
  struct RingBuffer *rb;
  struct Bank *bank;
};

struct Bank *initBank();
void resize(struct Bank *bank);
void addAccount(struct Bank *bank, int accountNumber, int initBalance);
void tellerInit(void *arg);
void printResult(struct Bank *bank);
