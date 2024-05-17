static int btreeInitPage(MemPage *pPage){

  assert( pPage->pBt!=0 );
  assert( pPage->pBt->db!=0 );
  assert( sqlite3_mutex_held(pPage->pBt->mutex) );
  assert( pPage->pgno==sqlite3PagerPagenumber(pPage->pDbPage) );
  assert( pPage == sqlite3PagerGetExtra(pPage->pDbPage) );
  assert( pPage->aData == sqlite3PagerGetData(pPage->pDbPage) );

  if( !pPage->isInit ){
    int pc;             
    u8 hdr;             
    u8 *data;           
    BtShared *pBt;         
    int usableSize;     
    u16 cellOffset;     
    int nFree;          
    int top;            
    int iCellFirst;     
    int iCellLast;      

    pBt = pPage->pBt;

    hdr = pPage->hdrOffset;
    data = pPage->aData;
     
    if( decodeFlags(pPage, data[hdr]) ) return SQLITE_CORRUPT_BKPT;
    assert( pBt->pageSize>=512 && pBt->pageSize<=65536 );
    pPage->maskPage = (u16)(pBt->pageSize - 1);
    pPage->nOverflow = 0;
    usableSize = pBt->usableSize;
    pPage->cellOffset = cellOffset = hdr + 8 + pPage->childPtrSize;
    pPage->aDataEnd = &data[usableSize];
    pPage->aCellIdx = &data[cellOffset];
    pPage->aDataOfst = &data[pPage->childPtrSize];
     
    top = get2byteNotZero(&data[hdr+5]);
     
    pPage->nCell = get2byte(&data[hdr+3]);
    if( pPage->nCell>MX_CELL(pBt) ){
       
      return SQLITE_CORRUPT_BKPT;
    }
    testcase( pPage->nCell==MX_CELL(pBt) );
     
    assert( pPage->nCell>0 || top==usableSize || CORRUPT_DB );

     
    iCellFirst = cellOffset + 2*pPage->nCell;
    iCellLast = usableSize - 4;
    if( pBt->db->flags & SQLITE_CellSizeCk ){
      int i;             
      int sz;            

      if( !pPage->leaf ) iCellLast--;
      for(i=0; i<pPage->nCell; i++){
        pc = get2byteAligned(&data[cellOffset+i*2]);
        testcase( pc==iCellFirst );
        testcase( pc==iCellLast );
        if( pc<iCellFirst || pc>iCellLast ){
          return SQLITE_CORRUPT_BKPT;
        }
        sz = pPage->xCellSize(pPage, &data[pc]);
        testcase( pc+sz==usableSize );
        if( pc+sz>usableSize ){
          return SQLITE_CORRUPT_BKPT;
        }
      }
      if( !pPage->leaf ) iCellLast++;
    }  

     
    pc = get2byte(&data[hdr+1]);
    nFree = data[hdr+7] + top;   
    if( pc>0 ){
      u32 next, size;
      if( pc<iCellFirst ){
         
        return SQLITE_CORRUPT_BKPT; 
      }
      while( 1 ){
        if( pc>iCellLast ){
          return SQLITE_CORRUPT_BKPT;  
        }
        next = get2byte(&data[pc]);
        size = get2byte(&data[pc+2]);
        nFree = nFree + size;
        if( next<=pc+size+3 ) break;
        pc = next;
      }
      if( next>0 ){
        return SQLITE_CORRUPT_BKPT;   
      }
      if( pc+size>(unsigned int)usableSize ){
        return SQLITE_CORRUPT_BKPT;   
      }
    }

     
    if( nFree>usableSize ){
      return SQLITE_CORRUPT_BKPT; 
    }
    pPage->nFree = (u16)(nFree - iCellFirst);
    pPage->isInit = 1;
  }
  return SQLITE_OK;
}
