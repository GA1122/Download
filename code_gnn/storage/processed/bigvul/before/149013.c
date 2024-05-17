static int btreeCursor(
  Btree *p,                               
  int iTable,                             
  int wrFlag,                             
  struct KeyInfo *pKeyInfo,               
  BtCursor *pCur                          
){
  BtShared *pBt = p->pBt;                 
  BtCursor *pX;                           

  assert( sqlite3BtreeHoldsMutex(p) );
  assert( wrFlag==0 
       || wrFlag==BTREE_WRCSR 
       || wrFlag==(BTREE_WRCSR|BTREE_FORDELETE) 
  );

   
  assert( hasSharedCacheTableLock(p, iTable, pKeyInfo!=0, (wrFlag?2:1)) );
  assert( wrFlag==0 || !hasReadConflicts(p, iTable) );

   
  assert( p->inTrans>TRANS_NONE );
  assert( wrFlag==0 || p->inTrans==TRANS_WRITE );
  assert( pBt->pPage1 && pBt->pPage1->aData );
  assert( wrFlag==0 || (pBt->btsFlags & BTS_READ_ONLY)==0 );

  if( wrFlag ){
    allocateTempSpace(pBt);
    if( pBt->pTmpSpace==0 ) return SQLITE_NOMEM_BKPT;
  }
  if( iTable==1 && btreePagecount(pBt)==0 ){
    assert( wrFlag==0 );
    iTable = 0;
  }

   
  pCur->pgnoRoot = (Pgno)iTable;
  pCur->iPage = -1;
  pCur->pKeyInfo = pKeyInfo;
  pCur->pBtree = p;
  pCur->pBt = pBt;
  pCur->curFlags = wrFlag ? BTCF_WriteFlag : 0;
  pCur->curPagerFlags = wrFlag ? 0 : PAGER_GET_READONLY;
   
  for(pX=pBt->pCursor; pX; pX=pX->pNext){
    if( pX->pgnoRoot==(Pgno)iTable ){
      pX->curFlags |= BTCF_Multiple;
      pCur->curFlags |= BTCF_Multiple;
    }
  }
  pCur->pNext = pBt->pCursor;
  pBt->pCursor = pCur;
  pCur->eState = CURSOR_INVALID;
  return SQLITE_OK;
}
