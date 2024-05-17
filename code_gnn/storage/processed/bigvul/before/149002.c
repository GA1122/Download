static int balance_nonroot(
  MemPage *pParent,                
  int iParentIdx,                  
  u8 *aOvflSpace,                  
  int isRoot,                      
  int bBulk                        
){
  BtShared *pBt;                
  int nMaxCells = 0;            
  int nNew = 0;                 
  int nOld;                     
  int i, j, k;                  
  int nxDiv;                    
  int rc = SQLITE_OK;           
  u16 leafCorrection;           
  int leafData;                 
  int usableSpace;              
  int pageFlags;                
  int iSpace1 = 0;              
  int iOvflSpace = 0;           
  int szScratch;                
  MemPage *apOld[NB];           
  MemPage *apNew[NB+2];         
  u8 *pRight;                   
  u8 *apDiv[NB-1];              
  int cntNew[NB+2];             
  int cntOld[NB+2];             
  int szNew[NB+2];              
  u8 *aSpace1;                  
  Pgno pgno;                    
  u8 abDone[NB+2];              
  Pgno aPgno[NB+2];             
  Pgno aPgOrder[NB+2];          
  u16 aPgFlags[NB+2];           
  CellArray b;                   

  memset(abDone, 0, sizeof(abDone));
  b.nCell = 0;
  b.apCell = 0;
  pBt = pParent->pBt;
  assert( sqlite3_mutex_held(pBt->mutex) );
  assert( sqlite3PagerIswriteable(pParent->pDbPage) );

#if 0
  TRACE(("BALANCE: begin page %d child of %d\n", pPage->pgno, pParent->pgno));
#endif

   
  assert( pParent->nOverflow==0 || pParent->nOverflow==1 );
  assert( pParent->nOverflow==0 || pParent->aiOvfl[0]==iParentIdx );

  if( !aOvflSpace ){
    return SQLITE_NOMEM_BKPT;
  }

   
  i = pParent->nOverflow + pParent->nCell;
  if( i<2 ){
    nxDiv = 0;
  }else{
    assert( bBulk==0 || bBulk==1 );
    if( iParentIdx==0 ){                 
      nxDiv = 0;
    }else if( iParentIdx==i ){
      nxDiv = i-2+bBulk;
    }else{
      nxDiv = iParentIdx-1;
    }
    i = 2-bBulk;
  }
  nOld = i+1;
  if( (i+nxDiv-pParent->nOverflow)==pParent->nCell ){
    pRight = &pParent->aData[pParent->hdrOffset+8];
  }else{
    pRight = findCell(pParent, i+nxDiv-pParent->nOverflow);
  }
  pgno = get4byte(pRight);
  while( 1 ){
    rc = getAndInitPage(pBt, pgno, &apOld[i], 0, 0);
    if( rc ){
      memset(apOld, 0, (i+1)*sizeof(MemPage*));
      goto balance_cleanup;
    }
    nMaxCells += 1+apOld[i]->nCell+apOld[i]->nOverflow;
    if( (i--)==0 ) break;

    if( pParent->nOverflow && i+nxDiv==pParent->aiOvfl[0] ){
      apDiv[i] = pParent->apOvfl[0];
      pgno = get4byte(apDiv[i]);
      szNew[i] = pParent->xCellSize(pParent, apDiv[i]);
      pParent->nOverflow = 0;
    }else{
      apDiv[i] = findCell(pParent, i+nxDiv-pParent->nOverflow);
      pgno = get4byte(apDiv[i]);
      szNew[i] = pParent->xCellSize(pParent, apDiv[i]);

       
      if( pBt->btsFlags & BTS_SECURE_DELETE ){
        int iOff;

        iOff = SQLITE_PTR_TO_INT(apDiv[i]) - SQLITE_PTR_TO_INT(pParent->aData);
        if( (iOff+szNew[i])>(int)pBt->usableSize ){
          rc = SQLITE_CORRUPT_BKPT;
          memset(apOld, 0, (i+1)*sizeof(MemPage*));
          goto balance_cleanup;
        }else{
          memcpy(&aOvflSpace[iOff], apDiv[i], szNew[i]);
          apDiv[i] = &aOvflSpace[apDiv[i]-pParent->aData];
        }
      }
      dropCell(pParent, i+nxDiv-pParent->nOverflow, szNew[i], &rc);
    }
  }

   
  nMaxCells = (nMaxCells + 3)&~3;

   
  szScratch =
       nMaxCells*sizeof(u8*)                        
     + nMaxCells*sizeof(u16)                        
     + pBt->pageSize;                               

   
  assert( szScratch<=6*(int)pBt->pageSize );
  b.apCell = sqlite3ScratchMalloc( szScratch ); 
  if( b.apCell==0 ){
    rc = SQLITE_NOMEM_BKPT;
    goto balance_cleanup;
  }
  b.szCell = (u16*)&b.apCell[nMaxCells];
  aSpace1 = (u8*)&b.szCell[nMaxCells];
  assert( EIGHT_BYTE_ALIGNMENT(aSpace1) );

   
  b.pRef = apOld[0];
  leafCorrection = b.pRef->leaf*4;
  leafData = b.pRef->intKeyLeaf;
  for(i=0; i<nOld; i++){
    MemPage *pOld = apOld[i];
    int limit = pOld->nCell;
    u8 *aData = pOld->aData;
    u16 maskPage = pOld->maskPage;
    u8 *piCell = aData + pOld->cellOffset;
    u8 *piEnd;

     
    if( pOld->aData[0]!=apOld[0]->aData[0] ){
      rc = SQLITE_CORRUPT_BKPT;
      goto balance_cleanup;
    }

     
    memset(&b.szCell[b.nCell], 0, sizeof(b.szCell[0])*(limit+pOld->nOverflow));
    if( pOld->nOverflow>0 ){
      limit = pOld->aiOvfl[0];
      for(j=0; j<limit; j++){
        b.apCell[b.nCell] = aData + (maskPage & get2byteAligned(piCell));
        piCell += 2;
        b.nCell++;
      }
      for(k=0; k<pOld->nOverflow; k++){
        assert( k==0 || pOld->aiOvfl[k-1]+1==pOld->aiOvfl[k] ); 
        b.apCell[b.nCell] = pOld->apOvfl[k];
        b.nCell++;
      }
    }
    piEnd = aData + pOld->cellOffset + 2*pOld->nCell;
    while( piCell<piEnd ){
      assert( b.nCell<nMaxCells );
      b.apCell[b.nCell] = aData + (maskPage & get2byteAligned(piCell));
      piCell += 2;
      b.nCell++;
    }

    cntOld[i] = b.nCell;
    if( i<nOld-1 && !leafData){
      u16 sz = (u16)szNew[i];
      u8 *pTemp;
      assert( b.nCell<nMaxCells );
      b.szCell[b.nCell] = sz;
      pTemp = &aSpace1[iSpace1];
      iSpace1 += sz;
      assert( sz<=pBt->maxLocal+23 );
      assert( iSpace1 <= (int)pBt->pageSize );
      memcpy(pTemp, apDiv[i], sz);
      b.apCell[b.nCell] = pTemp+leafCorrection;
      assert( leafCorrection==0 || leafCorrection==4 );
      b.szCell[b.nCell] = b.szCell[b.nCell] - leafCorrection;
      if( !pOld->leaf ){
        assert( leafCorrection==0 );
        assert( pOld->hdrOffset==0 );
         
        memcpy(b.apCell[b.nCell], &pOld->aData[8], 4);
      }else{
        assert( leafCorrection==4 );
        while( b.szCell[b.nCell]<4 ){
           
          assert( b.szCell[b.nCell]==3 || CORRUPT_DB );
          assert( b.apCell[b.nCell]==&aSpace1[iSpace1-3] || CORRUPT_DB );
          aSpace1[iSpace1++] = 0x00;
          b.szCell[b.nCell]++;
        }
      }
      b.nCell++;
    }
  }

   
  usableSpace = pBt->usableSize - 12 + leafCorrection;
  for(i=0; i<nOld; i++){
    MemPage *p = apOld[i];
    szNew[i] = usableSpace - p->nFree;
    for(j=0; j<p->nOverflow; j++){
      szNew[i] += 2 + p->xCellSize(p, p->apOvfl[j]);
    }
    cntNew[i] = cntOld[i];
  }
  k = nOld;
  for(i=0; i<k; i++){
    int sz;
    while( szNew[i]>usableSpace ){
      if( i+1>=k ){
        k = i+2;
        if( k>NB+2 ){ rc = SQLITE_CORRUPT_BKPT; goto balance_cleanup; }
        szNew[k-1] = 0;
        cntNew[k-1] = b.nCell;
      }
      sz = 2 + cachedCellSize(&b, cntNew[i]-1);
      szNew[i] -= sz;
      if( !leafData ){
        if( cntNew[i]<b.nCell ){
          sz = 2 + cachedCellSize(&b, cntNew[i]);
        }else{
          sz = 0;
        }
      }
      szNew[i+1] += sz;
      cntNew[i]--;
    }
    while( cntNew[i]<b.nCell ){
      sz = 2 + cachedCellSize(&b, cntNew[i]);
      if( szNew[i]+sz>usableSpace ) break;
      szNew[i] += sz;
      cntNew[i]++;
      if( !leafData ){
        if( cntNew[i]<b.nCell ){
          sz = 2 + cachedCellSize(&b, cntNew[i]);
        }else{
          sz = 0;
        }
      }
      szNew[i+1] -= sz;
    }
    if( cntNew[i]>=b.nCell ){
      k = i+1;
    }else if( cntNew[i] <= (i>0 ? cntNew[i-1] : 0) ){
      rc = SQLITE_CORRUPT_BKPT;
      goto balance_cleanup;
    }
  }

   
  for(i=k-1; i>0; i--){
    int szRight = szNew[i];   
    int szLeft = szNew[i-1];  
    int r;               
    int d;               

    r = cntNew[i-1] - 1;
    d = r + 1 - leafData;
    (void)cachedCellSize(&b, d);
    do{
      assert( d<nMaxCells );
      assert( r<nMaxCells );
      (void)cachedCellSize(&b, r);
      if( szRight!=0
       && (bBulk || szRight+b.szCell[d]+2 > szLeft-(b.szCell[r]+(i==k-1?0:2)))){
        break;
      }
      szRight += b.szCell[d] + 2;
      szLeft -= b.szCell[r] + 2;
      cntNew[i-1] = r;
      r--;
      d--;
    }while( r>=0 );
    szNew[i] = szRight;
    szNew[i-1] = szLeft;
    if( cntNew[i-1] <= (i>1 ? cntNew[i-2] : 0) ){
      rc = SQLITE_CORRUPT_BKPT;
      goto balance_cleanup;
    }
  }

   
  assert( cntNew[0]>0 || (pParent->pgno==1 && pParent->nCell==0) || CORRUPT_DB);
  TRACE(("BALANCE: old: %d(nc=%d) %d(nc=%d) %d(nc=%d)\n",
    apOld[0]->pgno, apOld[0]->nCell,
    nOld>=2 ? apOld[1]->pgno : 0, nOld>=2 ? apOld[1]->nCell : 0,
    nOld>=3 ? apOld[2]->pgno : 0, nOld>=3 ? apOld[2]->nCell : 0
  ));

   
  pageFlags = apOld[0]->aData[0];
  for(i=0; i<k; i++){
    MemPage *pNew;
    if( i<nOld ){
      pNew = apNew[i] = apOld[i];
      apOld[i] = 0;
      rc = sqlite3PagerWrite(pNew->pDbPage);
      nNew++;
      if( rc ) goto balance_cleanup;
    }else{
      assert( i>0 );
      rc = allocateBtreePage(pBt, &pNew, &pgno, (bBulk ? 1 : pgno), 0);
      if( rc ) goto balance_cleanup;
      zeroPage(pNew, pageFlags);
      apNew[i] = pNew;
      nNew++;
      cntOld[i] = b.nCell;

       
      if( ISAUTOVACUUM ){
        ptrmapPut(pBt, pNew->pgno, PTRMAP_BTREE, pParent->pgno, &rc);
        if( rc!=SQLITE_OK ){
          goto balance_cleanup;
        }
      }
    }
  }

   
  for(i=0; i<nNew; i++){
    aPgOrder[i] = aPgno[i] = apNew[i]->pgno;
    aPgFlags[i] = apNew[i]->pDbPage->flags;
    for(j=0; j<i; j++){
      if( aPgno[j]==aPgno[i] ){
         
        assert( CORRUPT_DB );
        rc = SQLITE_CORRUPT_BKPT;
        goto balance_cleanup;
      }
    }
  }
  for(i=0; i<nNew; i++){
    int iBest = 0;                 
    for(j=1; j<nNew; j++){
      if( aPgOrder[j]<aPgOrder[iBest] ) iBest = j;
    }
    pgno = aPgOrder[iBest];
    aPgOrder[iBest] = 0xffffffff;
    if( iBest!=i ){
      if( iBest>i ){
        sqlite3PagerRekey(apNew[iBest]->pDbPage, pBt->nPage+iBest+1, 0);
      }
      sqlite3PagerRekey(apNew[i]->pDbPage, pgno, aPgFlags[iBest]);
      apNew[i]->pgno = pgno;
    }
  }

  TRACE(("BALANCE: new: %d(%d nc=%d) %d(%d nc=%d) %d(%d nc=%d) "
         "%d(%d nc=%d) %d(%d nc=%d)\n",
    apNew[0]->pgno, szNew[0], cntNew[0],
    nNew>=2 ? apNew[1]->pgno : 0, nNew>=2 ? szNew[1] : 0,
    nNew>=2 ? cntNew[1] - cntNew[0] - !leafData : 0,
    nNew>=3 ? apNew[2]->pgno : 0, nNew>=3 ? szNew[2] : 0,
    nNew>=3 ? cntNew[2] - cntNew[1] - !leafData : 0,
    nNew>=4 ? apNew[3]->pgno : 0, nNew>=4 ? szNew[3] : 0,
    nNew>=4 ? cntNew[3] - cntNew[2] - !leafData : 0,
    nNew>=5 ? apNew[4]->pgno : 0, nNew>=5 ? szNew[4] : 0,
    nNew>=5 ? cntNew[4] - cntNew[3] - !leafData : 0
  ));

  assert( sqlite3PagerIswriteable(pParent->pDbPage) );
  put4byte(pRight, apNew[nNew-1]->pgno);

   
  if( (pageFlags & PTF_LEAF)==0 && nOld!=nNew ){
    MemPage *pOld = (nNew>nOld ? apNew : apOld)[nOld-1];
    memcpy(&apNew[nNew-1]->aData[8], &pOld->aData[8], 4);
  }

   
  if( ISAUTOVACUUM ){
    MemPage *pNew = apNew[0];
    u8 *aOld = pNew->aData;
    int cntOldNext = pNew->nCell + pNew->nOverflow;
    int usableSize = pBt->usableSize;
    int iNew = 0;
    int iOld = 0;

    for(i=0; i<b.nCell; i++){
      u8 *pCell = b.apCell[i];
      if( i==cntOldNext ){
        MemPage *pOld = (++iOld)<nNew ? apNew[iOld] : apOld[iOld];
        cntOldNext += pOld->nCell + pOld->nOverflow + !leafData;
        aOld = pOld->aData;
      }
      if( i==cntNew[iNew] ){
        pNew = apNew[++iNew];
        if( !leafData ) continue;
      }

       
      if( iOld>=nNew
       || pNew->pgno!=aPgno[iOld]
       || !SQLITE_WITHIN(pCell,aOld,&aOld[usableSize])
      ){
        if( !leafCorrection ){
          ptrmapPut(pBt, get4byte(pCell), PTRMAP_BTREE, pNew->pgno, &rc);
        }
        if( cachedCellSize(&b,i)>pNew->minLocal ){
          ptrmapPutOvflPtr(pNew, pCell, &rc);
        }
        if( rc ) goto balance_cleanup;
      }
    }
  }

   
  for(i=0; i<nNew-1; i++){
    u8 *pCell;
    u8 *pTemp;
    int sz;
    MemPage *pNew = apNew[i];
    j = cntNew[i];

    assert( j<nMaxCells );
    assert( b.apCell[j]!=0 );
    pCell = b.apCell[j];
    sz = b.szCell[j] + leafCorrection;
    pTemp = &aOvflSpace[iOvflSpace];
    if( !pNew->leaf ){
      memcpy(&pNew->aData[8], pCell, 4);
    }else if( leafData ){
       
      CellInfo info;
      j--;
      pNew->xParseCell(pNew, b.apCell[j], &info);
      pCell = pTemp;
      sz = 4 + putVarint(&pCell[4], info.nKey);
      pTemp = 0;
    }else{
      pCell -= 4;
       
      if( b.szCell[j]==4 ){
        assert(leafCorrection==4);
        sz = pParent->xCellSize(pParent, pCell);
      }
    }
    iOvflSpace += sz;
    assert( sz<=pBt->maxLocal+23 );
    assert( iOvflSpace <= (int)pBt->pageSize );
    insertCell(pParent, nxDiv+i, pCell, sz, pTemp, pNew->pgno, &rc);
    if( rc!=SQLITE_OK ) goto balance_cleanup;
    assert( sqlite3PagerIswriteable(pParent->pDbPage) );
  }

   
  for(i=1-nNew; i<nNew; i++){
    int iPg = i<0 ? -i : i;
    assert( iPg>=0 && iPg<nNew );
    if( abDone[iPg] ) continue;          
    if( i>=0                             
     || cntOld[iPg-1]>=cntNew[iPg-1]     
    ){
      int iNew;
      int iOld;
      int nNewCell;

       
      assert( iPg==0 || cntOld[iPg-1]>=cntNew[iPg-1] || abDone[iPg-1] );

       
      assert( cntNew[iPg]>=cntOld[iPg] || abDone[iPg+1] );

      if( iPg==0 ){
        iNew = iOld = 0;
        nNewCell = cntNew[0];
      }else{
        iOld = iPg<nOld ? (cntOld[iPg-1] + !leafData) : b.nCell;
        iNew = cntNew[iPg-1] + !leafData;
        nNewCell = cntNew[iPg] - iNew;
      }

      rc = editPage(apNew[iPg], iOld, iNew, nNewCell, &b);
      if( rc ) goto balance_cleanup;
      abDone[iPg]++;
      apNew[iPg]->nFree = usableSpace-szNew[iPg];
      assert( apNew[iPg]->nOverflow==0 );
      assert( apNew[iPg]->nCell==nNewCell );
    }
  }

   
  assert( memcmp(abDone, "\01\01\01\01\01", nNew)==0 );

  assert( nOld>0 );
  assert( nNew>0 );

  if( isRoot && pParent->nCell==0 && pParent->hdrOffset<=apNew[0]->nFree ){
     
    assert( nNew==1 || CORRUPT_DB );
    rc = defragmentPage(apNew[0]);
    testcase( rc!=SQLITE_OK );
    assert( apNew[0]->nFree == 
        (get2byte(&apNew[0]->aData[5])-apNew[0]->cellOffset-apNew[0]->nCell*2)
      || rc!=SQLITE_OK
    );
    copyNodeContent(apNew[0], pParent, &rc);
    freePage(apNew[0], &rc);
  }else if( ISAUTOVACUUM && !leafCorrection ){
     
    for(i=0; i<nNew; i++){
      u32 key = get4byte(&apNew[i]->aData[8]);
      ptrmapPut(pBt, key, PTRMAP_BTREE, apNew[i]->pgno, &rc);
    }
  }

  assert( pParent->isInit );
  TRACE(("BALANCE: finished: old=%d new=%d cells=%d\n",
          nOld, nNew, b.nCell));

   
  for(i=nNew; i<nOld; i++){
    freePage(apOld[i], &rc);
  }

#if 0
  if( ISAUTOVACUUM && rc==SQLITE_OK && apNew[0]->isInit ){
     
    ptrmapCheckPages(apNew, nNew);
    ptrmapCheckPages(&pParent, 1);
  }
#endif

   
balance_cleanup:
  sqlite3ScratchFree(b.apCell);
  for(i=0; i<nOld; i++){
    releasePage(apOld[i]);
  }
  for(i=0; i<nNew; i++){
    releasePage(apNew[i]);
  }

  return rc;
}
