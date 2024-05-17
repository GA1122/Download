static int dbpageOpen(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor){
  DbpageCursor *pCsr;

  pCsr = (DbpageCursor *)sqlite3_malloc64(sizeof(DbpageCursor));
  if( pCsr==0 ){
    return SQLITE_NOMEM_BKPT;
  }else{
    memset(pCsr, 0, sizeof(DbpageCursor));
    pCsr->base.pVtab = pVTab;
    pCsr->pgno = -1;
  }

  *ppCursor = (sqlite3_vtab_cursor *)pCsr;
  return SQLITE_OK;
}
