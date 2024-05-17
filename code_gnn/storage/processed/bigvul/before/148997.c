static int backupOnePage(
  sqlite3_backup *p,               
  Pgno iSrcPg,                     
  const u8 *zSrcData,              
  int bUpdate                      
){
  Pager * const pDestPager = sqlite3BtreePager(p->pDest);
  const int nSrcPgsz = sqlite3BtreeGetPageSize(p->pSrc);
  int nDestPgsz = sqlite3BtreeGetPageSize(p->pDest);
  const int nCopy = MIN(nSrcPgsz, nDestPgsz);
  const i64 iEnd = (i64)iSrcPg*(i64)nSrcPgsz;
#ifdef SQLITE_HAS_CODEC
   
  int nSrcReserve = sqlite3BtreeGetReserveNoMutex(p->pSrc);
  int nDestReserve = sqlite3BtreeGetOptimalReserve(p->pDest);
#endif
  int rc = SQLITE_OK;
  i64 iOff;

  assert( sqlite3BtreeGetReserveNoMutex(p->pSrc)>=0 );
  assert( p->bDestLocked );
  assert( !isFatalError(p->rc) );
  assert( iSrcPg!=PENDING_BYTE_PAGE(p->pSrc->pBt) );
  assert( zSrcData );

   
  if( nSrcPgsz!=nDestPgsz && sqlite3PagerIsMemdb(pDestPager) ){
    rc = SQLITE_READONLY;
  }

#ifdef SQLITE_HAS_CODEC
   
  if( nSrcPgsz!=nDestPgsz && sqlite3PagerGetCodec(pDestPager)!=0 ){
    rc = SQLITE_READONLY;
  }

   
  if( nSrcReserve!=nDestReserve ){
    u32 newPgsz = nSrcPgsz;
    rc = sqlite3PagerSetPagesize(pDestPager, &newPgsz, nSrcReserve);
    if( rc==SQLITE_OK && newPgsz!=nSrcPgsz ) rc = SQLITE_READONLY;
  }
#endif

   
  for(iOff=iEnd-(i64)nSrcPgsz; rc==SQLITE_OK && iOff<iEnd; iOff+=nDestPgsz){
    DbPage *pDestPg = 0;
    Pgno iDest = (Pgno)(iOff/nDestPgsz)+1;
    if( iDest==PENDING_BYTE_PAGE(p->pDest->pBt) ) continue;
    if( SQLITE_OK==(rc = sqlite3PagerGet(pDestPager, iDest, &pDestPg, 0))
     && SQLITE_OK==(rc = sqlite3PagerWrite(pDestPg))
    ){
      const u8 *zIn = &zSrcData[iOff%nSrcPgsz];
      u8 *zDestData = sqlite3PagerGetData(pDestPg);
      u8 *zOut = &zDestData[iOff%nDestPgsz];

       
      memcpy(zOut, zIn, nCopy);
      ((u8 *)sqlite3PagerGetExtra(pDestPg))[0] = 0;
      if( iOff==0 && bUpdate==0 ){
        sqlite3Put4byte(&zOut[28], sqlite3BtreeLastPage(p->pSrc));
      }
    }
    sqlite3PagerUnref(pDestPg);
  }

  return rc;
}
