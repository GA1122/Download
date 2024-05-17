static const void *columnName(
  sqlite3_stmt *pStmt,
  int N,
  const void *(*xFunc)(Mem*),
  int useType
){
  const void *ret;
  Vdbe *p;
  int n;
  sqlite3 *db;
#ifdef SQLITE_ENABLE_API_ARMOR
  if( pStmt==0 ){
    (void)SQLITE_MISUSE_BKPT;
    return 0;
  }
#endif
  ret = 0;
  p = (Vdbe *)pStmt;
  db = p->db;
  assert( db!=0 );
  n = sqlite3_column_count(pStmt);
  if( N<n && N>=0 ){
    N += useType*n;
    sqlite3_mutex_enter(db->mutex);
    assert( db->mallocFailed==0 );
    ret = xFunc(&p->aColName[N]);
      
    if( db->mallocFailed ){
      sqlite3OomClear(db);
      ret = 0;
    }
    sqlite3_mutex_leave(db->mutex);
  }
  return ret;
}
