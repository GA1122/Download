static int copyPayload(
  void *pPayload,            
  void *pBuf,                
  int nByte,                 
  int eOp,                   
  DbPage *pDbPage            
){
  if( eOp ){
     
    int rc = sqlite3PagerWrite(pDbPage);
    if( rc!=SQLITE_OK ){
      return rc;
    }
    memcpy(pPayload, pBuf, nByte);
  }else{
     
    memcpy(pBuf, pPayload, nByte);
  }
  return SQLITE_OK;
}
