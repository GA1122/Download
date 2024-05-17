static int dbpageBegin(sqlite3_vtab *pVtab){
  DbpageTable *pTab = (DbpageTable *)pVtab;
  sqlite3 *db = pTab->db;
  int i;
  for(i=0; i<db->nDb; i++){
    Btree *pBt = db->aDb[i].pBt;
    if( pBt ) sqlite3BtreeBeginTrans(pBt, 1, 0);
  }
  return SQLITE_OK;
}
