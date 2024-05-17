static int checkRef(IntegrityCk *pCheck, Pgno iPage){
  if( iPage==0 ) return 1;
  if( iPage>pCheck->nPage ){
    checkAppendMsg(pCheck, "invalid page number %d", iPage);
    return 1;
  }
  if( getPageReferenced(pCheck, iPage) ){
    checkAppendMsg(pCheck, "2nd reference to page %d", iPage);
    return 1;
  }
  setPageReferenced(pCheck, iPage);
  return 0;
}
