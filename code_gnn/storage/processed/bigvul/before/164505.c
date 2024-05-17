static SQLITE_NOINLINE int btreePrevious(BtCursor *pCur){
  int rc;
  MemPage *pPage;

  assert( cursorOwnsBtShared(pCur) );
  assert( (pCur->curFlags & (BTCF_AtLast|BTCF_ValidOvfl|BTCF_ValidNKey))==0 );
  assert( pCur->info.nSize==0 );
  if( pCur->eState!=CURSOR_VALID ){
    rc = restoreCursorPosition(pCur);
    if( rc!=SQLITE_OK ){
      return rc;
    }
    if( CURSOR_INVALID==pCur->eState ){
      return SQLITE_DONE;
    }
    if( CURSOR_SKIPNEXT==pCur->eState ){
      pCur->eState = CURSOR_VALID;
      if( pCur->skipNext<0 ) return SQLITE_OK;
    }
  }

  pPage = pCur->pPage;
  assert( pPage->isInit );
  if( !pPage->leaf ){
    int idx = pCur->ix;
    rc = moveToChild(pCur, get4byte(findCell(pPage, idx)));
    if( rc ) return rc;
    rc = moveToRightmost(pCur);
  }else{
    while( pCur->ix==0 ){
      if( pCur->iPage==0 ){
        pCur->eState = CURSOR_INVALID;
        return SQLITE_DONE;
      }
      moveToParent(pCur);
    }
    assert( pCur->info.nSize==0 );
    assert( (pCur->curFlags & (BTCF_ValidOvfl))==0 );

    pCur->ix--;
    pPage = pCur->pPage;
    if( pPage->intKey && !pPage->leaf ){
      rc = sqlite3BtreePrevious(pCur, 0);
    }else{
      rc = SQLITE_OK;
    }
  }
  return rc;
}
