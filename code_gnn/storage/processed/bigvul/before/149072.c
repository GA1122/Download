static int clearDatabasePage(
  BtShared *pBt,            
  Pgno pgno,                
  int freePageFlag,         
  int *pnChange             
){
  MemPage *pPage;
  int rc;
  unsigned char *pCell;
  int i;
  int hdr;
  CellInfo info;

  assert( sqlite3_mutex_held(pBt->mutex) );
  if( pgno>btreePagecount(pBt) ){
    return SQLITE_CORRUPT_BKPT;
  }
  rc = getAndInitPage(pBt, pgno, &pPage, 0, 0);
  if( rc ) return rc;
  if( pPage->bBusy ){
    rc = SQLITE_CORRUPT_BKPT;
    goto cleardatabasepage_out;
  }
  pPage->bBusy = 1;
  hdr = pPage->hdrOffset;
  for(i=0; i<pPage->nCell; i++){
    pCell = findCell(pPage, i);
    if( !pPage->leaf ){
      rc = clearDatabasePage(pBt, get4byte(pCell), 1, pnChange);
      if( rc ) goto cleardatabasepage_out;
    }
    rc = clearCell(pPage, pCell, &info);
    if( rc ) goto cleardatabasepage_out;
  }
  if( !pPage->leaf ){
    rc = clearDatabasePage(pBt, get4byte(&pPage->aData[hdr+8]), 1, pnChange);
    if( rc ) goto cleardatabasepage_out;
  }else if( pnChange ){
    assert( pPage->intKey || CORRUPT_DB );
    testcase( !pPage->intKey );
    *pnChange += pPage->nCell;
  }
  if( freePageFlag ){
    freePage(pPage, &rc);
  }else if( (rc = sqlite3PagerWrite(pPage->pDbPage))==0 ){
    zeroPage(pPage, pPage->aData[hdr] | PTF_LEAF);
  }

cleardatabasepage_out:
  pPage->bBusy = 0;
  releasePage(pPage);
  return rc;
}
