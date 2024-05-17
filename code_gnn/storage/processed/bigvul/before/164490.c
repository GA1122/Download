static int blobReadWrite(
  sqlite3_blob *pBlob,
  void *z,
  int n,
  int iOffset,
  int (*xCall)(BtCursor*, u32, u32, void*)
){
  int rc;
  Incrblob *p = (Incrblob *)pBlob;
  Vdbe *v;
  sqlite3 *db;

  if( p==0 ) return SQLITE_MISUSE_BKPT;
  db = p->db;
  sqlite3_mutex_enter(db->mutex);
  v = (Vdbe*)p->pStmt;

  if( n<0 || iOffset<0 || ((sqlite3_int64)iOffset+n)>p->nByte ){
     
    rc = SQLITE_ERROR;
  }else if( v==0 ){
     
    rc = SQLITE_ABORT;
  }else{
     
    assert( db == v->db );
    sqlite3BtreeEnterCursor(p->pCsr);

#ifdef SQLITE_ENABLE_PREUPDATE_HOOK
    if( xCall==sqlite3BtreePutData && db->xPreUpdateCallback ){
       
      sqlite3_int64 iKey;
      iKey = sqlite3BtreeIntegerKey(p->pCsr);
      sqlite3VdbePreUpdateHook(
          v, v->apCsr[0], SQLITE_DELETE, p->zDb, p->pTab, iKey, -1
      );
    }
#endif

    rc = xCall(p->pCsr, iOffset+p->iOffset, n, z);
    sqlite3BtreeLeaveCursor(p->pCsr);
    if( rc==SQLITE_ABORT ){
      sqlite3VdbeFinalize(v);
      p->pStmt = 0;
    }else{
      v->rc = rc;
    }
  }
  sqlite3Error(db, rc);
  rc = sqlite3ApiExit(db, rc);
  sqlite3_mutex_leave(db->mutex);
  return rc;
}
