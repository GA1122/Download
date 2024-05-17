static int autoVacuumCommit(BtShared *pBt){
  int rc = SQLITE_OK;
  int bShouldVacuum = pBt->autoVacuum && !pBt->incrVacuum;
  Pager *pPager = pBt->pPager;
  VVA_ONLY( int nRef = sqlite3PagerRefcount(pPager); )

  assert( sqlite3_mutex_held(pBt->mutex) );
  invalidateAllOverflowCache(pBt);
  assert(pBt->autoVacuum);
  if( bShouldVacuum && pBt->autoVacuumSlack ){
    Pgno nOrig;         
    Pgno nFree;         

    nOrig = btreePagecount(pBt);
    nFree = get4byte(&pBt->pPage1->aData[36]);
    bShouldVacuum =
        (nOrig-nFree-1)/pBt->autoVacuumSlack < (nOrig-1)/pBt->autoVacuumSlack;
     
  }
  if( bShouldVacuum ){
    Pgno nFin;          
    Pgno nFree;         
    Pgno iFree;         
    Pgno nOrig;         

    nOrig = btreePagecount(pBt);
    if( PTRMAP_ISPAGE(pBt, nOrig) || nOrig==PENDING_BYTE_PAGE(pBt) ){
       
      return SQLITE_CORRUPT_BKPT;
    }

    nFree = get4byte(&pBt->pPage1->aData[36]);
    nFin = finalDbSize(pBt, nOrig, nFree);
    if( nFin>nOrig ) return SQLITE_CORRUPT_BKPT;
    if( nFin<nOrig ){
      rc = saveAllCursors(pBt, 0, 0);
    }
    for(iFree=nOrig; iFree>nFin && rc==SQLITE_OK; iFree--){
      rc = incrVacuumStep(pBt, nFin, iFree, 1);
    }
    if( (rc==SQLITE_DONE || rc==SQLITE_OK) && nFree>0 ){
      rc = sqlite3PagerWrite(pBt->pPage1->pDbPage);
      put4byte(&pBt->pPage1->aData[32], 0);
      put4byte(&pBt->pPage1->aData[36], 0);
      put4byte(&pBt->pPage1->aData[28], nFin);
      pBt->bDoTruncate = 1;
      pBt->nPage = nFin;
    }
    if( rc!=SQLITE_OK ){
      sqlite3PagerRollback(pPager);
    }
  }

  assert( nRef>=sqlite3PagerRefcount(pPager) );
  return rc;
}
