static int afpClose(sqlite3_file *id) {
  int rc = SQLITE_OK;
  unixFile *pFile = (unixFile*)id;
  assert( id!=0 );
  afpUnlock(id, NO_LOCK);
  unixEnterMutex();
  if( pFile->pInode && pFile->pInode->nLock ){
     
    setPendingFd(pFile);
  }
  releaseInodeInfo(pFile);
  sqlite3_free(pFile->lockingContext);
  rc = closeUnixFile(id);
  unixLeaveMutex();
  return rc;
}
