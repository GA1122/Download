static void SQLITE_NOINLINE btreeLockCarefully(Btree *p){
  Btree *pLater;

   
  if( sqlite3_mutex_try(p->pBt->mutex)==SQLITE_OK ){
    p->pBt->db = p->db;
    p->locked = 1;
    return;
  }

   
  for(pLater=p->pNext; pLater; pLater=pLater->pNext){
    assert( pLater->sharable );
    assert( pLater->pNext==0 || pLater->pNext->pBt>pLater->pBt );
    assert( !pLater->locked || pLater->wantToLock>0 );
    if( pLater->locked ){
      unlockBtreeMutex(pLater);
    }
  }
  lockBtreeMutex(p);
  for(pLater=p->pNext; pLater; pLater=pLater->pNext){
    if( pLater->wantToLock ){
      lockBtreeMutex(pLater);
    }
  }
}
