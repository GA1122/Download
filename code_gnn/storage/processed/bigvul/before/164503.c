static int btreeOverwriteContent(
  MemPage *pPage,            
  u8 *pDest,                 
  const BtreePayload *pX,    
  int iOffset,               
  int iAmt                   
){
  int nData = pX->nData - iOffset;
  if( nData<=0 ){
     
    int i;
    for(i=0; i<iAmt && pDest[i]==0; i++){}
    if( i<iAmt ){
      int rc = sqlite3PagerWrite(pPage->pDbPage);
      if( rc ) return rc;
      memset(pDest + i, 0, iAmt - i);
    }
  }else{
    if( nData<iAmt ){
       
      int rc = btreeOverwriteContent(pPage, pDest+nData, pX, iOffset+nData,
                                 iAmt-nData);
      if( rc ) return rc;
      iAmt = nData;
    }
    if( memcmp(pDest, ((u8*)pX->pData) + iOffset, iAmt)!=0 ){
      int rc = sqlite3PagerWrite(pPage->pDbPage);
      if( rc ) return rc;
       
      memmove(pDest, ((u8*)pX->pData) + iOffset, iAmt);
    }
  }
  return SQLITE_OK;
}
