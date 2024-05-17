static int dbpageNext(sqlite3_vtab_cursor *pCursor){
  int rc = SQLITE_OK;
  DbpageCursor *pCsr = (DbpageCursor *)pCursor;
  pCsr->pgno++;
  return rc;
}
