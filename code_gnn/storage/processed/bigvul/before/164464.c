static void allocateTempSpace(BtShared *pBt){
  if( !pBt->pTmpSpace ){
    pBt->pTmpSpace = sqlite3PageMalloc( pBt->pageSize );

     
    if( pBt->pTmpSpace ){
      memset(pBt->pTmpSpace, 0, 8);
      pBt->pTmpSpace += 4;
    }
  }
}
