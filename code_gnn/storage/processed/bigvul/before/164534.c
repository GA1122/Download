static void codeCursorHint(
  struct SrcList_item *pTabItem,   
  WhereInfo *pWInfo,     
  WhereLevel *pLevel,    
  WhereTerm *pEndRange   
){
  Parse *pParse = pWInfo->pParse;
  sqlite3 *db = pParse->db;
  Vdbe *v = pParse->pVdbe;
  Expr *pExpr = 0;
  WhereLoop *pLoop = pLevel->pWLoop;
  int iCur;
  WhereClause *pWC;
  WhereTerm *pTerm;
  int i, j;
  struct CCurHint sHint;
  Walker sWalker;

  if( OptimizationDisabled(db, SQLITE_CursorHints) ) return;
  iCur = pLevel->iTabCur;
  assert( iCur==pWInfo->pTabList->a[pLevel->iFrom].iCursor );
  sHint.iTabCur = iCur;
  sHint.iIdxCur = pLevel->iIdxCur;
  sHint.pIdx = pLoop->u.btree.pIndex;
  memset(&sWalker, 0, sizeof(sWalker));
  sWalker.pParse = pParse;
  sWalker.u.pCCurHint = &sHint;
  pWC = &pWInfo->sWC;
  for(i=0; i<pWC->nTerm; i++){
    pTerm = &pWC->a[i];
    if( pTerm->wtFlags & (TERM_VIRTUAL|TERM_CODED) ) continue;
    if( pTerm->prereqAll & pLevel->notReady ) continue;

     
    if( pTabItem->fg.jointype & JT_LEFT ){
      Expr *pExpr = pTerm->pExpr;
      if( !ExprHasProperty(pExpr, EP_FromJoin)
       || pExpr->iRightJoinTable!=pTabItem->iCursor
      ){
        sWalker.eCode = 0;
        sWalker.xExprCallback = codeCursorHintIsOrFunction;
        sqlite3WalkExpr(&sWalker, pTerm->pExpr);
        if( sWalker.eCode ) continue;
      }
    }else{
      if( ExprHasProperty(pTerm->pExpr, EP_FromJoin) ) continue;
    }

     
    if( pLoop->u.btree.nEq==0 && pTerm!=pEndRange ){
      for(j=0; j<pLoop->nLTerm && pLoop->aLTerm[j]!=pTerm; j++){}
      if( j<pLoop->nLTerm ) continue;
    }

     
    if( sqlite3ExprContainsSubquery(pTerm->pExpr) ) continue;

     
    if( sHint.pIdx!=0 ){
      sWalker.eCode = 0;
      sWalker.xExprCallback = codeCursorHintCheckExpr;
      sqlite3WalkExpr(&sWalker, pTerm->pExpr);
      if( sWalker.eCode ) continue;
    }

     
    pExpr = sqlite3ExprAnd(db, pExpr, sqlite3ExprDup(db, pTerm->pExpr, 0));
  }
  if( pExpr!=0 ){
    sWalker.xExprCallback = codeCursorHintFixExpr;
    sqlite3WalkExpr(&sWalker, pExpr);
    sqlite3VdbeAddOp4(v, OP_CursorHint,
                      (sHint.pIdx ? sHint.iIdxCur : sHint.iTabCur), 0, 0,
                      (const char*)pExpr, P4_EXPR);
  }
}
