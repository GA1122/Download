int chromium_sqlite3_fill_in_unix_sqlite3_file(sqlite3_vfs* pVfs,
                                               int fd,
                                               sqlite3_file* pFile,
                                               const char* zPath,
                                               int noLock,
                                               int flags) {
  unixFile *p = (unixFile *)pFile;
  const int eType = flags&0xFFFFFF00;   
  const int ctrlFlags = (noLock ? UNIXFILE_NOLOCK : 0);
  int rc;

  memset(p, 0, sizeof(unixFile));

   
  if( eType==SQLITE_OPEN_MAIN_DB ){
    p->pUnused = sqlite3_malloc(sizeof(*p->pUnused));
    if (!p->pUnused) {
      return SQLITE_NOMEM_BKPT;
    }
    p->pUnused->fd = fd;
    p->pUnused->flags = flags;
  }

  rc = fillInUnixFile(pVfs, fd, pFile, zPath, ctrlFlags);
  if( rc!=SQLITE_OK ){
    sqlite3_free(p->pUnused);
  }
  return rc;
}
