static int accessPayload(
  BtCursor *pCur,       
  u32 offset,           
  u32 amt,              
  unsigned char *pBuf,   
  int eOp               
){
  unsigned char *aPayload;
  int rc = SQLITE_OK;
  int iIdx = 0;
  MemPage *pPage = pCur->apPage[pCur->iPage];  
  BtShared *pBt = pCur->pBt;                   
#ifdef SQLITE_DIRECT_OVERFLOW_READ
  unsigned char * const pBufStart = pBuf;      
#endif

  assert( pPage );
  assert( eOp==0 || eOp==1 );
  assert( pCur->eState==CURSOR_VALID );
  assert( pCur->aiIdx[pCur->iPage]<pPage->nCell );
  assert( cursorHoldsMutex(pCur) );

  getCellInfo(pCur);
  aPayload = pCur->info.pPayload;
  assert( offset+amt <= pCur->info.nPayload );

  assert( aPayload > pPage->aData );
  if( (uptr)(aPayload - pPage->aData) > (pBt->usableSize - pCur->info.nLocal) ){
     
    return SQLITE_CORRUPT_BKPT;
  }

   
  if( offset<pCur->info.nLocal ){
    int a = amt;
    if( a+offset>pCur->info.nLocal ){
      a = pCur->info.nLocal - offset;
    }
    rc = copyPayload(&aPayload[offset], pBuf, a, eOp, pPage->pDbPage);
    offset = 0;
    pBuf += a;
    amt -= a;
  }else{
    offset -= pCur->info.nLocal;
  }


  if( rc==SQLITE_OK && amt>0 ){
    const u32 ovflSize = pBt->usableSize - 4;   
    Pgno nextPage;

    nextPage = get4byte(&aPayload[pCur->info.nLocal]);

     
    if( (pCur->curFlags & BTCF_ValidOvfl)==0 ){
      int nOvfl = (pCur->info.nPayload-pCur->info.nLocal+ovflSize-1)/ovflSize;
      if( nOvfl>pCur->nOvflAlloc ){
        Pgno *aNew = (Pgno*)sqlite3Realloc(
            pCur->aOverflow, nOvfl*2*sizeof(Pgno)
        );
        if( aNew==0 ){
          return SQLITE_NOMEM_BKPT;
        }else{
          pCur->nOvflAlloc = nOvfl*2;
          pCur->aOverflow = aNew;
        }
      }
      memset(pCur->aOverflow, 0, nOvfl*sizeof(Pgno));
      pCur->curFlags |= BTCF_ValidOvfl;
    }else{
       
      if( pCur->aOverflow[offset/ovflSize] ){
        iIdx = (offset/ovflSize);
        nextPage = pCur->aOverflow[iIdx];
        offset = (offset%ovflSize);
      }
    }

    assert( rc==SQLITE_OK && amt>0 );
    while( nextPage ){
       
      assert( pCur->aOverflow[iIdx]==0
              || pCur->aOverflow[iIdx]==nextPage
              || CORRUPT_DB );
      pCur->aOverflow[iIdx] = nextPage;

      if( offset>=ovflSize ){
         
        assert( pCur->curFlags & BTCF_ValidOvfl );
        assert( pCur->pBtree->db==pBt->db );
        if( pCur->aOverflow[iIdx+1] ){
          nextPage = pCur->aOverflow[iIdx+1];
        }else{
          rc = getOverflowPage(pBt, nextPage, 0, &nextPage);
        }
        offset -= ovflSize;
      }else{
         
#ifdef SQLITE_DIRECT_OVERFLOW_READ
        sqlite3_file *fd;       
#endif
        int a = amt;
        if( a + offset > ovflSize ){
          a = ovflSize - offset;
        }

#ifdef SQLITE_DIRECT_OVERFLOW_READ
         
        if( eOp==0                                              
         && offset==0                                           
         && pBt->inTransaction==TRANS_READ                      
         && (fd = sqlite3PagerFile(pBt->pPager))->pMethods      
         && 0==sqlite3PagerUseWal(pBt->pPager, nextPage)        
         && &pBuf[-4]>=pBufStart                                
        ){
          u8 aSave[4];
          u8 *aWrite = &pBuf[-4];
          assert( aWrite>=pBufStart );                          
          memcpy(aSave, aWrite, 4);
          rc = sqlite3OsRead(fd, aWrite, a+4, (i64)pBt->pageSize*(nextPage-1));
          nextPage = get4byte(aWrite);
          memcpy(aWrite, aSave, 4);
        }else
#endif

        {
          DbPage *pDbPage;
          rc = sqlite3PagerGet(pBt->pPager, nextPage, &pDbPage,
              (eOp==0 ? PAGER_GET_READONLY : 0)
          );
          if( rc==SQLITE_OK ){
            aPayload = sqlite3PagerGetData(pDbPage);
            nextPage = get4byte(aPayload);
            rc = copyPayload(&aPayload[offset+4], pBuf, a, eOp, pDbPage);
            sqlite3PagerUnref(pDbPage);
            offset = 0;
          }
        }
        amt -= a;
        if( amt==0 ) return rc;
        pBuf += a;
      }
      if( rc ) break;
      iIdx++;
    }
  }

  if( rc==SQLITE_OK && amt>0 ){
    return SQLITE_CORRUPT_BKPT;  
  }
  return rc;
}
