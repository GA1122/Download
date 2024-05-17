static int checkMutexInit(void){
  pGlobalMutexMethods = sqlite3DefaultMutex();
  return SQLITE_OK;
}
