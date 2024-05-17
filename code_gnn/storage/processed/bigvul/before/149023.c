static void SQLITE_NOINLINE btreeLeaveAll(sqlite3 *db){
  int i;
  Btree *p;
  assert( sqlite3_mutex_held(db->mutex) );
  for(i=0; i<db->nDb; i++){
    p = db->aDb[i].pBt;
    if( p ) sqlite3BtreeLeave(p);
  }
}
