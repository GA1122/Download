static void checkPtrmap(
  IntegrityCk *pCheck,    
  Pgno iChild,            
  u8 eType,               
  Pgno iParent            
){
  int rc;
  u8 ePtrmapType;
  Pgno iPtrmapParent;

  rc = ptrmapGet(pCheck->pBt, iChild, &ePtrmapType, &iPtrmapParent);
  if( rc!=SQLITE_OK ){
    if( rc==SQLITE_NOMEM || rc==SQLITE_IOERR_NOMEM ) pCheck->mallocFailed = 1;
    checkAppendMsg(pCheck, "Failed to read ptrmap key=%d", iChild);
    return;
  }

  if( ePtrmapType!=eType || iPtrmapParent!=iParent ){
    checkAppendMsg(pCheck,
      "Bad ptr map entry key=%d expected=(%d,%d) got=(%d,%d)",
      iChild, eType, iParent, ePtrmapType, iPtrmapParent);
  }
}
