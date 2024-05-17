static int codeCursorHintFixExpr(Walker *pWalker, Expr *pExpr){
  int rc = WRC_Continue;
  struct CCurHint *pHint = pWalker->u.pCCurHint;
  if( pExpr->op==TK_COLUMN ){
    if( pExpr->iTable!=pHint->iTabCur ){
      Vdbe *v = pWalker->pParse->pVdbe;
      int reg = ++pWalker->pParse->nMem;    
      sqlite3ExprCodeGetColumnOfTable(
          v, pExpr->pTab, pExpr->iTable, pExpr->iColumn, reg
      );
      pExpr->op = TK_REGISTER;
      pExpr->iTable = reg;
    }else if( pHint->pIdx!=0 ){
      pExpr->iTable = pHint->iIdxCur;
      pExpr->iColumn = sqlite3ColumnOfIndex(pHint->pIdx, pExpr->iColumn);
      assert( pExpr->iColumn>=0 );
    }
  }else if( pExpr->op==TK_AGG_FUNCTION ){
     
    rc = WRC_Prune;
  }
  return rc;
}
