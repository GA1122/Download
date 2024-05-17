static int dbpageConnect(
  sqlite3 *db,
  void *pAux,
  int argc, const char *const*argv,
  sqlite3_vtab **ppVtab,
  char **pzErr
){
  DbpageTable *pTab = 0;
  int rc = SQLITE_OK;

  rc = sqlite3_declare_vtab(db,
          "CREATE TABLE x(pgno INTEGER PRIMARY KEY, data BLOB, schema HIDDEN)");
  if( rc==SQLITE_OK ){
    pTab = (DbpageTable *)sqlite3_malloc64(sizeof(DbpageTable));
    if( pTab==0 ) rc = SQLITE_NOMEM_BKPT;
  }

  assert( rc==SQLITE_OK || pTab==0 );
  if( rc==SQLITE_OK ){
    memset(pTab, 0, sizeof(DbpageTable));
    pTab->db = db;
  }

  *ppVtab = (sqlite3_vtab*)pTab;
  return rc;
}
