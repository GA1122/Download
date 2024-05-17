static void btreeReleaseAllCursorPages(BtCursor *pCur){
  int i;
  for(i=0; i<=pCur->iPage; i++){
    releasePage(pCur->apPage[i]);
    pCur->apPage[i] = 0;
  }
  pCur->iPage = -1;
}
