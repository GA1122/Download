static int checkMutexNotheld(sqlite3_mutex *p){
  return pGlobalMutexMethods->xMutexNotheld(((CheckMutex*)p)->mutex);
}
