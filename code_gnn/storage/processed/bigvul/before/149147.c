static void debugMutexFree(sqlite3_mutex *pX){
  sqlite3_debug_mutex *p = (sqlite3_debug_mutex*)pX;
  assert( p->cnt==0 );
  if( p->id==SQLITE_MUTEX_RECURSIVE || p->id==SQLITE_MUTEX_FAST ){
    sqlite3_free(p);
  }else{
#ifdef SQLITE_ENABLE_API_ARMOR
    (void)SQLITE_MISUSE_BKPT;
#endif
  }
}
