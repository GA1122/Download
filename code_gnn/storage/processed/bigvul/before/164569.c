static int dbpageColumn(
  sqlite3_vtab_cursor *pCursor,
  sqlite3_context *ctx,
  int i
){
  DbpageCursor *pCsr = (DbpageCursor *)pCursor;
  int rc = SQLITE_OK;
  switch( i ){
    case 0: {            
      sqlite3_result_int(ctx, pCsr->pgno);
      break;
    }
    case 1: {            
      DbPage *pDbPage = 0;
      rc = sqlite3PagerGet(pCsr->pPager, pCsr->pgno, (DbPage**)&pDbPage, 0);
      if( rc==SQLITE_OK ){
        sqlite3_result_blob(ctx, sqlite3PagerGetData(pDbPage), pCsr->szPage,
                            SQLITE_TRANSIENT);
      }
      sqlite3PagerUnref(pDbPage);
      break;
    }
    default: {           
      sqlite3 *db = sqlite3_context_db_handle(ctx);
      sqlite3_result_text(ctx, db->aDb[pCsr->iDb].zDbSName, -1, SQLITE_STATIC);
      break;
    }
  }
  return SQLITE_OK;
}
