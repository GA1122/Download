static int allocateSpace(MemPage *pPage, int nByte, int *pIdx){
  const int hdr = pPage->hdrOffset;     
  u8 * const data = pPage->aData;       
  int top;                              
  int rc = SQLITE_OK;                   
  int gap;         

  assert( sqlite3PagerIswriteable(pPage->pDbPage) );
  assert( pPage->pBt );
  assert( sqlite3_mutex_held(pPage->pBt->mutex) );
  assert( nByte>=0 );   
  assert( pPage->nFree>=nByte );
  assert( pPage->nOverflow==0 );
  assert( nByte < (int)(pPage->pBt->usableSize-8) );

  assert( pPage->cellOffset == hdr + 12 - 4*pPage->leaf );
  gap = pPage->cellOffset + 2*pPage->nCell;
  assert( gap<=65536 );
   
  top = get2byte(&data[hdr+5]);
  assert( top<=(int)pPage->pBt->usableSize );  
  if( gap>top ){
    if( top==0 && pPage->pBt->usableSize==65536 ){
      top = 65536;
    }else{
      return SQLITE_CORRUPT_PAGE(pPage);
    }
  }

   
  testcase( gap+2==top );
  testcase( gap+1==top );
  testcase( gap==top );
  if( (data[hdr+2] || data[hdr+1]) && gap+2<=top ){
    u8 *pSpace = pageFindSlot(pPage, nByte, &rc);
    if( pSpace ){
      assert( pSpace>=data && (pSpace - data)<65536 );
      *pIdx = (int)(pSpace - data);
      return SQLITE_OK;
    }else if( rc ){
      return rc;
    }
  }

   
  testcase( gap+2+nByte==top );
  if( gap+2+nByte>top ){
    assert( pPage->nCell>0 || CORRUPT_DB );
    rc = defragmentPage(pPage, MIN(4, pPage->nFree - (2+nByte)));
    if( rc ) return rc;
    top = get2byteNotZero(&data[hdr+5]);
    assert( gap+2+nByte<=top );
  }


   
  top -= nByte;
  put2byte(&data[hdr+5], top);
  assert( top+nByte <= (int)pPage->pBt->usableSize );
  *pIdx = top;
  return SQLITE_OK;
}
