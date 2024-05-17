static u16 cellSizePtr(MemPage *pPage, u8 *pCell){
  u8 *pIter = pCell + pPage->childPtrSize;  
  u8 *pEnd;                                 
  u32 nSize;                                

#ifdef SQLITE_DEBUG
   
  CellInfo debuginfo;
  pPage->xParseCell(pPage, pCell, &debuginfo);
#endif

  nSize = *pIter;
  if( nSize>=0x80 ){
    pEnd = &pIter[8];
    nSize &= 0x7f;
    do{
      nSize = (nSize<<7) | (*++pIter & 0x7f);
    }while( *(pIter)>=0x80 && pIter<pEnd );
  }
  pIter++;
  if( pPage->intKey ){
     
    pEnd = &pIter[9];
    while( (*pIter++)&0x80 && pIter<pEnd );
  }
  testcase( nSize==pPage->maxLocal );
  testcase( nSize==pPage->maxLocal+1 );
  if( nSize<=pPage->maxLocal ){
    nSize += (u32)(pIter - pCell);
    if( nSize<4 ) nSize = 4;
  }else{
    int minLocal = pPage->minLocal;
    nSize = minLocal + (nSize - minLocal) % (pPage->pBt->usableSize - 4);
    testcase( nSize==pPage->maxLocal );
    testcase( nSize==pPage->maxLocal+1 );
    if( nSize>pPage->maxLocal ){
      nSize = minLocal;
    }
    nSize += 4 + (u16)(pIter - pCell);
  }
  assert( nSize==debuginfo.nSize || CORRUPT_DB );
  return (u16)nSize;
}
