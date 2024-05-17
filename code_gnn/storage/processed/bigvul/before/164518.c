static void checkMutexEnter(sqlite3_mutex *p){
  CheckMutex *pCheck = (CheckMutex*)p;
  if( pCheck->iType==SQLITE_MUTEX_WARNONCONTENTION ){
    if( SQLITE_OK==pGlobalMutexMethods->xMutexTry(pCheck->mutex) ){
      return;
    }
    sqlite3_log(SQLITE_MISUSE,
        "illegal multi-threaded access to database connection"
    );
  }
  pGlobalMutexMethods->xMutexEnter(pCheck->mutex);
}
