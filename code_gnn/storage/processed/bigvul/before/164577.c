static int dbpageUpdate(
  sqlite3_vtab *pVtab,
  int argc,
  sqlite3_value **argv,
  sqlite_int64 *pRowid
){
  DbpageTable *pTab = (DbpageTable *)pVtab;
  Pgno pgno;
  DbPage *pDbPage = 0;
  int rc = SQLITE_OK;
  char *zErr = 0;
  const char *zSchema;
  int iDb;
  Btree *pBt;
  Pager *pPager;
  int szPage;

  if( pTab->db->flags & SQLITE_Defensive ){
    zErr = "read-only";
    goto update_fail;
  }
  if( argc==1 ){
    zErr = "cannot delete";
    goto update_fail;
  }
  pgno = sqlite3_value_int(argv[0]);
  if( (Pgno)sqlite3_value_int(argv[1])!=pgno ){
    zErr = "cannot insert";
    goto update_fail;
  }
  zSchema = (const char*)sqlite3_value_text(argv[4]);
  iDb = zSchema ? sqlite3FindDbName(pTab->db, zSchema) : -1;
  if( iDb<0 ){
    zErr = "no such schema";
    goto update_fail;
  }
  pBt = pTab->db->aDb[iDb].pBt;
  if( pgno<1 || pBt==0 || pgno>(int)sqlite3BtreeLastPage(pBt) ){
    zErr = "bad page number";
    goto update_fail;
  }
  szPage = sqlite3BtreeGetPageSize(pBt);
  if( sqlite3_value_type(argv[3])!=SQLITE_BLOB
   || sqlite3_value_bytes(argv[3])!=szPage
  ){
    zErr = "bad page value";
    goto update_fail;
  }
  pPager = sqlite3BtreePager(pBt);
  rc = sqlite3PagerGet(pPager, pgno, (DbPage**)&pDbPage, 0);
  if( rc==SQLITE_OK ){
    rc = sqlite3PagerWrite(pDbPage);
    if( rc==SQLITE_OK ){
      memcpy(sqlite3PagerGetData(pDbPage),
             sqlite3_value_blob(argv[3]),
             szPage);
    }
  }
  sqlite3PagerUnref(pDbPage);
  return rc;

update_fail:
  sqlite3_free(pVtab->zErrMsg);
  pVtab->zErrMsg = sqlite3_mprintf("%s", zErr);
  return SQLITE_ERROR;
}
