static void btreeParseCellPtr(
  MemPage *pPage,          
  u8 *pCell,               
  CellInfo *pInfo          
){
  u8 *pIter;               
  u32 nPayload;            
  u64 iKey;                

  assert( sqlite3_mutex_held(pPage->pBt->mutex) );
  assert( pPage->leaf==0 || pPage->leaf==1 );
  assert( pPage->intKeyLeaf );
  assert( pPage->childPtrSize==0 );
  pIter = pCell;

   
  nPayload = *pIter;
  if( nPayload>=0x80 ){
    u8 *pEnd = &pIter[8];
    nPayload &= 0x7f;
    do{
      nPayload = (nPayload<<7) | (*++pIter & 0x7f);
    }while( (*pIter)>=0x80 && pIter<pEnd );
  }
  pIter++;

   
  iKey = *pIter;
  if( iKey>=0x80 ){
    u8 *pEnd = &pIter[7];
    iKey &= 0x7f;
    while(1){
      iKey = (iKey<<7) | (*++pIter & 0x7f);
      if( (*pIter)<0x80 ) break;
      if( pIter>=pEnd ){
        iKey = (iKey<<8) | *++pIter;
        break;
      }
    }
  }
  pIter++;

  pInfo->nKey = *(i64*)&iKey;
  pInfo->nPayload = nPayload;
  pInfo->pPayload = pIter;
  testcase( nPayload==pPage->maxLocal );
  testcase( nPayload==pPage->maxLocal+1 );
  if( nPayload<=pPage->maxLocal ){
     
    pInfo->nSize = nPayload + (u16)(pIter - pCell);
    if( pInfo->nSize<4 ) pInfo->nSize = 4;
    pInfo->nLocal = (u16)nPayload;
  }else{
    btreeParseCellAdjustSizeForOverflow(pPage, pCell, pInfo);
  }
}
