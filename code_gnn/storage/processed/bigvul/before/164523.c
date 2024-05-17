static int checkMutexTry(sqlite3_mutex *p){
  CheckMutex *pCheck = (CheckMutex*)p;
  return pGlobalMutexMethods->xMutexTry(pCheck->mutex);
}
