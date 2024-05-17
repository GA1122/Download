static sqlite3_mutex *checkMutexAlloc(int iType){
  static CheckMutex staticMutexes[] = {
    {2, 0}, {3, 0}, {4, 0}, {5, 0},
    {6, 0}, {7, 0}, {8, 0}, {9, 0},
    {10, 0}, {11, 0}, {12, 0}, {13, 0}
  };
  CheckMutex *p = 0;

  assert( SQLITE_MUTEX_RECURSIVE==1 && SQLITE_MUTEX_FAST==0 );
  if( iType<2 ){
    p = sqlite3MallocZero(sizeof(CheckMutex));
    if( p==0 ) return 0;
    p->iType = iType;
  }else{
#ifdef SQLITE_ENABLE_API_ARMOR
    if( iType-2>=ArraySize(staticMutexes) ){
      (void)SQLITE_MISUSE_BKPT;
      return 0;
    }
#endif
    p = &staticMutexes[iType-2];
  }

  if( p->mutex==0 ){
    p->mutex = pGlobalMutexMethods->xMutexAlloc(iType);
    if( p->mutex==0 ){
      if( iType<2 ){
        sqlite3_free(p);
      }
      p = 0;
    }
  }

  return (sqlite3_mutex*)p;
}
