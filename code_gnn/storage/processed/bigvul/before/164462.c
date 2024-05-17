static sqlite3_index_info *allocateIndexInfo(
  Parse *pParse,                   
  WhereClause *pWC,                
  Bitmask mUnusable,               
  struct SrcList_item *pSrc,       
  ExprList *pOrderBy,              
  u16 *pmNoOmit                    
){
  int i, j;
  int nTerm;
  struct sqlite3_index_constraint *pIdxCons;
  struct sqlite3_index_orderby *pIdxOrderBy;
  struct sqlite3_index_constraint_usage *pUsage;
  struct HiddenIndexInfo *pHidden;
  WhereTerm *pTerm;
  int nOrderBy;
  sqlite3_index_info *pIdxInfo;
  u16 mNoOmit = 0;

   
  for(i=nTerm=0, pTerm=pWC->a; i<pWC->nTerm; i++, pTerm++){
    if( pTerm->leftCursor != pSrc->iCursor ) continue;
    if( pTerm->prereqRight & mUnusable ) continue;
    assert( IsPowerOfTwo(pTerm->eOperator & ~WO_EQUIV) );
    testcase( pTerm->eOperator & WO_IN );
    testcase( pTerm->eOperator & WO_ISNULL );
    testcase( pTerm->eOperator & WO_IS );
    testcase( pTerm->eOperator & WO_ALL );
    if( (pTerm->eOperator & ~(WO_EQUIV))==0 ) continue;
    if( pTerm->wtFlags & TERM_VNULL ) continue;
    assert( pTerm->u.leftColumn>=(-1) );
    nTerm++;
  }

   
  nOrderBy = 0;
  if( pOrderBy ){
    int n = pOrderBy->nExpr;
    for(i=0; i<n; i++){
      Expr *pExpr = pOrderBy->a[i].pExpr;
      if( pExpr->op!=TK_COLUMN || pExpr->iTable!=pSrc->iCursor ) break;
    }
    if( i==n){
      nOrderBy = n;
    }
  }

   
  pIdxInfo = sqlite3DbMallocZero(pParse->db, sizeof(*pIdxInfo)
                           + (sizeof(*pIdxCons) + sizeof(*pUsage))*nTerm
                           + sizeof(*pIdxOrderBy)*nOrderBy + sizeof(*pHidden) );
  if( pIdxInfo==0 ){
    sqlite3ErrorMsg(pParse, "out of memory");
    return 0;
  }

   
  pHidden = (struct HiddenIndexInfo*)&pIdxInfo[1];
  pIdxCons = (struct sqlite3_index_constraint*)&pHidden[1];
  pIdxOrderBy = (struct sqlite3_index_orderby*)&pIdxCons[nTerm];
  pUsage = (struct sqlite3_index_constraint_usage*)&pIdxOrderBy[nOrderBy];
  *(int*)&pIdxInfo->nConstraint = nTerm;
  *(int*)&pIdxInfo->nOrderBy = nOrderBy;
  *(struct sqlite3_index_constraint**)&pIdxInfo->aConstraint = pIdxCons;
  *(struct sqlite3_index_orderby**)&pIdxInfo->aOrderBy = pIdxOrderBy;
  *(struct sqlite3_index_constraint_usage**)&pIdxInfo->aConstraintUsage =
                                                                   pUsage;

  pHidden->pWC = pWC;
  pHidden->pParse = pParse;
  for(i=j=0, pTerm=pWC->a; i<pWC->nTerm; i++, pTerm++){
    u16 op;
    if( pTerm->leftCursor != pSrc->iCursor ) continue;
    if( pTerm->prereqRight & mUnusable ) continue;
    assert( IsPowerOfTwo(pTerm->eOperator & ~WO_EQUIV) );
    testcase( pTerm->eOperator & WO_IN );
    testcase( pTerm->eOperator & WO_IS );
    testcase( pTerm->eOperator & WO_ISNULL );
    testcase( pTerm->eOperator & WO_ALL );
    if( (pTerm->eOperator & ~(WO_EQUIV))==0 ) continue;
    if( pTerm->wtFlags & TERM_VNULL ) continue;
    if( (pSrc->fg.jointype & JT_LEFT)!=0
     && !ExprHasProperty(pTerm->pExpr, EP_FromJoin)
     && (pTerm->eOperator & (WO_IS|WO_ISNULL))
    ){
       
      testcase( pTerm->eOperator & WO_ISNULL );
      testcase( pTerm->eOperator & WO_IS );
      continue;
    }
    assert( pTerm->u.leftColumn>=(-1) );
    pIdxCons[j].iColumn = pTerm->u.leftColumn;
    pIdxCons[j].iTermOffset = i;
    op = pTerm->eOperator & WO_ALL;
    if( op==WO_IN ) op = WO_EQ;
    if( op==WO_AUX ){
      pIdxCons[j].op = pTerm->eMatchOp;
    }else if( op & (WO_ISNULL|WO_IS) ){
      if( op==WO_ISNULL ){
        pIdxCons[j].op = SQLITE_INDEX_CONSTRAINT_ISNULL;
      }else{
        pIdxCons[j].op = SQLITE_INDEX_CONSTRAINT_IS;
      }
    }else{
      pIdxCons[j].op = (u8)op;
       
      assert( WO_EQ==SQLITE_INDEX_CONSTRAINT_EQ );
      assert( WO_LT==SQLITE_INDEX_CONSTRAINT_LT );
      assert( WO_LE==SQLITE_INDEX_CONSTRAINT_LE );
      assert( WO_GT==SQLITE_INDEX_CONSTRAINT_GT );
      assert( WO_GE==SQLITE_INDEX_CONSTRAINT_GE );
      assert( pTerm->eOperator&(WO_IN|WO_EQ|WO_LT|WO_LE|WO_GT|WO_GE|WO_AUX) );

      if( op & (WO_LT|WO_LE|WO_GT|WO_GE)
       && sqlite3ExprIsVector(pTerm->pExpr->pRight)
      ){
        if( i<16 ) mNoOmit |= (1 << i);
        if( op==WO_LT ) pIdxCons[j].op = WO_LE;
        if( op==WO_GT ) pIdxCons[j].op = WO_GE;
      }
    }

    j++;
  }
  for(i=0; i<nOrderBy; i++){
    Expr *pExpr = pOrderBy->a[i].pExpr;
    pIdxOrderBy[i].iColumn = pExpr->iColumn;
    pIdxOrderBy[i].desc = pOrderBy->a[i].sortOrder;
  }

  *pmNoOmit = mNoOmit;
  return pIdxInfo;
}
