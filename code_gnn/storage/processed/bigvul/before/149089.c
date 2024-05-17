static int codeEqualityTerm(
  Parse *pParse,       
  WhereTerm *pTerm,    
  WhereLevel *pLevel,  
  int iEq,             
  int bRev,            
  int iTarget          
){
  Expr *pX = pTerm->pExpr;
  Vdbe *v = pParse->pVdbe;
  int iReg;                   

  assert( pLevel->pWLoop->aLTerm[iEq]==pTerm );
  assert( iTarget>0 );
  if( pX->op==TK_EQ || pX->op==TK_IS ){
    iReg = sqlite3ExprCodeTarget(pParse, pX->pRight, iTarget);
  }else if( pX->op==TK_ISNULL ){
    iReg = iTarget;
    sqlite3VdbeAddOp2(v, OP_Null, 0, iReg);
#ifndef SQLITE_OMIT_SUBQUERY
  }else{
    int eType = IN_INDEX_NOOP;
    int iTab;
    struct InLoop *pIn;
    WhereLoop *pLoop = pLevel->pWLoop;
    int i;
    int nEq = 0;
    int *aiMap = 0;

    if( (pLoop->wsFlags & WHERE_VIRTUALTABLE)==0
      && pLoop->u.btree.pIndex!=0
      && pLoop->u.btree.pIndex->aSortOrder[iEq]
    ){
      testcase( iEq==0 );
      testcase( bRev );
      bRev = !bRev;
    }
    assert( pX->op==TK_IN );
    iReg = iTarget;

    for(i=0; i<iEq; i++){
      if( pLoop->aLTerm[i] && pLoop->aLTerm[i]->pExpr==pX ){
        disableTerm(pLevel, pTerm);
        return iTarget;
      }
    }
    for(i=iEq;i<pLoop->nLTerm; i++){
      if( ALWAYS(pLoop->aLTerm[i]) && pLoop->aLTerm[i]->pExpr==pX ) nEq++;
    }

    if( (pX->flags & EP_xIsSelect)==0 || pX->x.pSelect->pEList->nExpr==1 ){
      eType = sqlite3FindInIndex(pParse, pX, IN_INDEX_LOOP, 0, 0);
    }else{
      Select *pSelect = pX->x.pSelect;
      sqlite3 *db = pParse->db;
      u16 savedDbOptFlags = db->dbOptFlags;
      ExprList *pOrigRhs = pSelect->pEList;
      ExprList *pOrigLhs = pX->pLeft->x.pList;
      ExprList *pRhs = 0;          
      ExprList *pLhs = 0;          

      for(i=iEq;i<pLoop->nLTerm; i++){
        if( pLoop->aLTerm[i]->pExpr==pX ){
          int iField = pLoop->aLTerm[i]->iField - 1;
          Expr *pNewRhs = sqlite3ExprDup(db, pOrigRhs->a[iField].pExpr, 0);
          Expr *pNewLhs = sqlite3ExprDup(db, pOrigLhs->a[iField].pExpr, 0);

          pRhs = sqlite3ExprListAppend(pParse, pRhs, pNewRhs);
          pLhs = sqlite3ExprListAppend(pParse, pLhs, pNewLhs);
        }
      }
      if( !db->mallocFailed ){
        Expr *pLeft = pX->pLeft;

        if( pSelect->pOrderBy ){
           
          ExprList *pOrderBy = pSelect->pOrderBy;
          for(i=0; i<pOrderBy->nExpr; i++){
            pOrderBy->a[i].u.x.iOrderByCol = 0;
          }
        }

         
        if( pLhs->nExpr==1 ){
          pX->pLeft = pLhs->a[0].pExpr;
        }else{
          pLeft->x.pList = pLhs;
          aiMap = (int*)sqlite3DbMallocZero(pParse->db, sizeof(int) * nEq);
          testcase( aiMap==0 );
        }
        pSelect->pEList = pRhs;
        db->dbOptFlags |= SQLITE_QueryFlattener;
        eType = sqlite3FindInIndex(pParse, pX, IN_INDEX_LOOP, 0, aiMap);
        db->dbOptFlags = savedDbOptFlags;
        testcase( aiMap!=0 && aiMap[0]!=0 );
        pSelect->pEList = pOrigRhs;
        pLeft->x.pList = pOrigLhs;
        pX->pLeft = pLeft;
      }
      sqlite3ExprListDelete(pParse->db, pLhs);
      sqlite3ExprListDelete(pParse->db, pRhs);
    }

    if( eType==IN_INDEX_INDEX_DESC ){
      testcase( bRev );
      bRev = !bRev;
    }
    iTab = pX->iTable;
    sqlite3VdbeAddOp2(v, bRev ? OP_Last : OP_Rewind, iTab, 0);
    VdbeCoverageIf(v, bRev);
    VdbeCoverageIf(v, !bRev);
    assert( (pLoop->wsFlags & WHERE_MULTI_OR)==0 );

    pLoop->wsFlags |= WHERE_IN_ABLE;
    if( pLevel->u.in.nIn==0 ){
      pLevel->addrNxt = sqlite3VdbeMakeLabel(v);
    }

    i = pLevel->u.in.nIn;
    pLevel->u.in.nIn += nEq;
    pLevel->u.in.aInLoop =
       sqlite3DbReallocOrFree(pParse->db, pLevel->u.in.aInLoop,
                              sizeof(pLevel->u.in.aInLoop[0])*pLevel->u.in.nIn);
    pIn = pLevel->u.in.aInLoop;
    if( pIn ){
      int iMap = 0;                
      pIn += i;
      for(i=iEq;i<pLoop->nLTerm; i++){
        if( pLoop->aLTerm[i]->pExpr==pX ){
          int iOut = iReg + i - iEq;
          if( eType==IN_INDEX_ROWID ){
            testcase( nEq>1 );   
            pIn->addrInTop = sqlite3VdbeAddOp2(v, OP_Rowid, iTab, iOut);
          }else{
            int iCol = aiMap ? aiMap[iMap++] : 0;
            pIn->addrInTop = sqlite3VdbeAddOp3(v,OP_Column,iTab, iCol, iOut);
          }
          sqlite3VdbeAddOp1(v, OP_IsNull, iOut); VdbeCoverage(v);
          if( i==iEq ){
            pIn->iCur = iTab;
            pIn->eEndLoopOp = bRev ? OP_PrevIfOpen : OP_NextIfOpen;
          }else{
            pIn->eEndLoopOp = OP_Noop;
          }
          pIn++;
        }
      }
    }else{
      pLevel->u.in.nIn = 0;
    }
    sqlite3DbFree(pParse->db, aiMap);
#endif
  }
  disableTerm(pLevel, pTerm);
  return iReg;
}
