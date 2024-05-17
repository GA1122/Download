static void convertToWithoutRowidTable(Parse *pParse, Table *pTab){
  Index *pIdx;
  Index *pPk;
  int nPk;
  int i, j;
  sqlite3 *db = pParse->db;
  Vdbe *v = pParse->pVdbe;

   
  if( !db->init.imposterTable ){
    for(i=0; i<pTab->nCol; i++){
      if( (pTab->aCol[i].colFlags & COLFLAG_PRIMKEY)!=0 ){
        pTab->aCol[i].notNull = OE_Abort;
      }
    }
  }

   
  if( pParse->addrCrTab ){
    assert( v );
    sqlite3VdbeChangeP3(v, pParse->addrCrTab, BTREE_BLOBKEY);
  }

   
  if( pTab->iPKey>=0 ){
    ExprList *pList;
    Token ipkToken;
    sqlite3TokenInit(&ipkToken, pTab->aCol[pTab->iPKey].zName);
    pList = sqlite3ExprListAppend(pParse, 0,
                  sqlite3ExprAlloc(db, TK_ID, &ipkToken, 0));
    if( pList==0 ) return;
    pList->a[0].sortOrder = pParse->iPkSortOrder;
    assert( pParse->pNewTable==pTab );
    sqlite3CreateIndex(pParse, 0, 0, 0, pList, pTab->keyConf, 0, 0, 0, 0,
                       SQLITE_IDXTYPE_PRIMARYKEY);
    if( db->mallocFailed || pParse->nErr ) return;
    pPk = sqlite3PrimaryKeyIndex(pTab);
    pTab->iPKey = -1;
  }else{
    pPk = sqlite3PrimaryKeyIndex(pTab);

     
    for(i=j=1; i<pPk->nKeyCol; i++){
      if( hasColumn(pPk->aiColumn, j, pPk->aiColumn[i]) ){
        pPk->nColumn--;
      }else{
        pPk->aiColumn[j++] = pPk->aiColumn[i];
      }
    }
    pPk->nKeyCol = j;
  }
  assert( pPk!=0 );
  pPk->isCovering = 1;
  if( !db->init.imposterTable ) pPk->uniqNotNull = 1;
  nPk = pPk->nKeyCol;

   
  if( v && pPk->tnum>0 ){
    assert( db->init.busy==0 );
    sqlite3VdbeChangeOpcode(v, pPk->tnum, OP_Goto);
  }

   
  pPk->tnum = pTab->tnum;

   
  for(pIdx=pTab->pIndex; pIdx; pIdx=pIdx->pNext){
    int n;
    if( IsPrimaryKeyIndex(pIdx) ) continue;
    for(i=n=0; i<nPk; i++){
      if( !hasColumn(pIdx->aiColumn, pIdx->nKeyCol, pPk->aiColumn[i]) ) n++;
    }
    if( n==0 ){
       
      pIdx->nColumn = pIdx->nKeyCol;
      continue;
    }
    if( resizeIndexObject(db, pIdx, pIdx->nKeyCol+n) ) return;
    for(i=0, j=pIdx->nKeyCol; i<nPk; i++){
      if( !hasColumn(pIdx->aiColumn, pIdx->nKeyCol, pPk->aiColumn[i]) ){
        pIdx->aiColumn[j] = pPk->aiColumn[i];
        pIdx->azColl[j] = pPk->azColl[i];
        j++;
      }
    }
    assert( pIdx->nColumn>=pIdx->nKeyCol+n );
    assert( pIdx->nColumn>=j );
  }

   
  if( nPk<pTab->nCol ){
    if( resizeIndexObject(db, pPk, pTab->nCol) ) return;
    for(i=0, j=nPk; i<pTab->nCol; i++){
      if( !hasColumn(pPk->aiColumn, j, i) ){
        assert( j<pPk->nColumn );
        pPk->aiColumn[j] = i;
        pPk->azColl[j] = sqlite3StrBINARY;
        j++;
      }
    }
    assert( pPk->nColumn==j );
    assert( pTab->nCol==j );
  }else{
    pPk->nColumn = pTab->nCol;
  }
  recomputeColumnsNotIndexed(pPk);
}
