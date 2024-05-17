static void codeExprOrVector(Parse *pParse, Expr *p, int iReg, int nReg){
  assert( nReg>0 );
  if( sqlite3ExprIsVector(p) ){
#ifndef SQLITE_OMIT_SUBQUERY
    if( (p->flags & EP_xIsSelect) ){
      Vdbe *v = pParse->pVdbe;
      int iSelect = sqlite3CodeSubselect(pParse, p, 0, 0);
      sqlite3VdbeAddOp3(v, OP_Copy, iSelect, iReg, nReg-1);
    }else
#endif
    {
      int i;
      ExprList *pList = p->x.pList;
      assert( nReg<=pList->nExpr );
      for(i=0; i<nReg; i++){
        sqlite3ExprCode(pParse, pList->a[i].pExpr, iReg+i);
      }
    }
  }else{
    assert( nReg==1 );
    sqlite3ExprCode(pParse, p, iReg);
  }
}
