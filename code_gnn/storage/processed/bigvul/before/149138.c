static int databaseIsUnmoved(Pager *pPager){
  int bHasMoved = 0;
  int rc;

  if( pPager->tempFile ) return SQLITE_OK;
  if( pPager->dbSize==0 ) return SQLITE_OK;
  assert( pPager->zFilename && pPager->zFilename[0] );
  rc = sqlite3OsFileControl(pPager->fd, SQLITE_FCNTL_HAS_MOVED, &bHasMoved);
  if( rc==SQLITE_NOTFOUND ){
     
    rc = SQLITE_OK;
  }else if( rc==SQLITE_OK && bHasMoved ){
    rc = SQLITE_READONLY_DBMOVED;
  }
  return rc;
}
