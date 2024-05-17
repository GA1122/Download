static SQLITE_NOINLINE int btreePrevious(BtCursor *pCur, int *pRes){
  int rc;
  MemPage *pPage;

  assert( cursorOwnsBtShared(pCur) );
  assert( pRes!=0 );
  assert( *pRes==0 );
  assert( pCur->skipNext==0 || pCur->eState!=CURSOR_VALID );
  assert( (pCur->curFlags & (BTCF_AtLast|BTCF_ValidOvfl|BTCF_ValidNKey))==0 );
  assert( pCur->info.nSize==0 );
  if( pCur->eState!=CURSOR_VALID ){
    rc = restoreCursorPosition(pCur);
    if( rc!=SQLITE_OK ){
      return rc;
    }
    if( CURSOR_INVALID==pCur->eState ){
      *pRes = 1;
      return SQLITE_OK;
    }
    if( pCur->skipNext ){
      assert( pCur->eState==CURSOR_VALID || pCur->eState==CURSOR_SKIPNEXT );
      pCur->eState = CURSOR_VALID;
      if( pCur->skipNext<0 ){
        pCur->skipNext = 0;
        return SQLITE_OK;
      }
      pCur->skipNext = 0;
    }
  }

  pPage = pCur->apPage[pCur->iPage];
  assert( pPage->isInit );
  if( !pPage->leaf ){
    int idx = pCur->aiIdx[pCur->iPage];
    rc = moveToChild(pCur, get4byte(findCell(pPage, idx)));
    if( rc ) return rc;
    rc = moveToRightmost(pCur);
  }else{
    while( pCur->aiIdx[pCur->iPage]==0 ){
      if( pCur->iPage==0 ){
        pCur->eState = CURSOR_INVALID;
        *pRes = 1;
        return SQLITE_OK;
      }
      moveToParent(pCur);
    }
    assert( pCur->info.nSize==0 );
    assert( (pCur->curFlags & (BTCF_ValidOvfl))==0 );

    pCur->aiIdx[pCur->iPage]--;
    pPage = pCur->apPage[pCur->iPage];
    if( pPage->intKey && !pPage->leaf ){
      rc = sqlite3BtreePrevious(pCur, pRes);
    }else{
      rc = SQLITE_OK;
    }
  }
  return rc;
}
