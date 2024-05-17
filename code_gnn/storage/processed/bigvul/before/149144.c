static sqlite3_mutex *debugMutexAlloc(int id){
  static sqlite3_debug_mutex aStatic[SQLITE_MUTEX_STATIC_VFS3 - 1];
  sqlite3_debug_mutex *pNew = 0;
  switch( id ){
    case SQLITE_MUTEX_FAST:
    case SQLITE_MUTEX_RECURSIVE: {
      pNew = sqlite3Malloc(sizeof(*pNew));
      if( pNew ){
        pNew->id = id;
        pNew->cnt = 0;
      }
      break;
    }
    default: {
#ifdef SQLITE_ENABLE_API_ARMOR
      if( id-2<0 || id-2>=ArraySize(aStatic) ){
        (void)SQLITE_MISUSE_BKPT;
        return 0;
      }
#endif
      pNew = &aStatic[id-2];
      pNew->id = id;
      break;
    }
  }
  return (sqlite3_mutex*)pNew;
}
