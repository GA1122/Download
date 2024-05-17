static int btreeMoveto(
  BtCursor *pCur,      
  const void *pKey,    
  i64 nKey,            
  int bias,            
  int *pRes            
){
  int rc;                     
  UnpackedRecord *pIdxKey;    

  if( pKey ){
    assert( nKey==(i64)(int)nKey );
    pIdxKey = sqlite3VdbeAllocUnpackedRecord(pCur->pKeyInfo);
    if( pIdxKey==0 ) return SQLITE_NOMEM_BKPT;
    sqlite3VdbeRecordUnpack(pCur->pKeyInfo, (int)nKey, pKey, pIdxKey);
    if( pIdxKey->nField==0 ){
      rc = SQLITE_CORRUPT_BKPT;
      goto moveto_done;
    }
  }else{
    pIdxKey = 0;
  }
  rc = sqlite3BtreeMovetoUnpacked(pCur, pIdxKey, nKey, bias, pRes);
moveto_done:
  if( pIdxKey ){
    sqlite3DbFree(pCur->pKeyInfo->db, pIdxKey);
  }
  return rc;
}
