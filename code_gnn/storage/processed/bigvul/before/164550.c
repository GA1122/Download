static void constInsert(
  WhereConst *pConst,       
  Expr *pColumn,            
  Expr *pValue              
){
  int i;
  assert( pColumn->op==TK_COLUMN );

   
  for(i=0; i<pConst->nConst; i++){
    const Expr *pExpr = pConst->apExpr[i*2];
    assert( pExpr->op==TK_COLUMN );
    if( pExpr->iTable==pColumn->iTable
     && pExpr->iColumn==pColumn->iColumn
    ){
      return;   
    }
  }

  pConst->nConst++;
  pConst->apExpr = sqlite3DbReallocOrFree(pConst->pParse->db, pConst->apExpr,
                         pConst->nConst*2*sizeof(Expr*));
  if( pConst->apExpr==0 ){
    pConst->nConst = 0;
  }else{
    if( ExprHasProperty(pValue, EP_FixedCol) ) pValue = pValue->pLeft;
    pConst->apExpr[pConst->nConst*2-2] = pColumn;
    pConst->apExpr[pConst->nConst*2-1] = pValue;
  }
}
