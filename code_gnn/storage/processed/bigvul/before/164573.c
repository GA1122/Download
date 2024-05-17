static int dbpageFilter(
  sqlite3_vtab_cursor *pCursor,
  int idxNum, const char *idxStr,
  int argc, sqlite3_value **argv
){
  DbpageCursor *pCsr = (DbpageCursor *)pCursor;
  DbpageTable *pTab = (DbpageTable *)pCursor->pVtab;
  int rc;
  sqlite3 *db = pTab->db;
  Btree *pBt;

   
  pCsr->pgno = 1;
  pCsr->mxPgno = 0;

  if( idxNum & 2 ){
    const char *zSchema;
    assert( argc>=1 );
    zSchema = (const char*)sqlite3_value_text(argv[0]);
    pCsr->iDb = sqlite3FindDbName(db, zSchema);
    if( pCsr->iDb<0 ) return SQLITE_OK;
  }else{
    pCsr->iDb = 0;
  }
  pBt = db->aDb[pCsr->iDb].pBt;
  if( pBt==0 ) return SQLITE_OK;
  pCsr->pPager = sqlite3BtreePager(pBt);
  pCsr->szPage = sqlite3BtreeGetPageSize(pBt);
  pCsr->mxPgno = sqlite3BtreeLastPage(pBt);
  if( idxNum & 1 ){
    assert( argc>(idxNum>>1) );
    pCsr->pgno = sqlite3_value_int(argv[idxNum>>1]);
    if( pCsr->pgno<1 || pCsr->pgno>pCsr->mxPgno ){
      pCsr->pgno = 1;
      pCsr->mxPgno = 0;
    }else{
      pCsr->mxPgno = pCsr->pgno;
    }
  }else{
    assert( pCsr->pgno==1 );
  }
  if( pCsr->pPage1 ) sqlite3PagerUnrefPageOne(pCsr->pPage1);
  rc = sqlite3PagerGet(pCsr->pPager, 1, &pCsr->pPage1, 0);
  return rc;
}
