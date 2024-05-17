static void btreeReleaseAllCursorPages(BtCursor *pCur){
  int i;
  if( pCur->iPage>=0 ){
    for(i=0; i<pCur->iPage; i++){
      releasePageNotNull(pCur->apPage[i]);
    }
    releasePageNotNull(pCur->pPage);
    pCur->iPage = -1;
  }
}
