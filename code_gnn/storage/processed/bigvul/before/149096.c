static int codeTriggerProgram(
  Parse *pParse,             
  TriggerStep *pStepList,    
  int orconf                   
){
  TriggerStep *pStep;
  Vdbe *v = pParse->pVdbe;
  sqlite3 *db = pParse->db;

  assert( pParse->pTriggerTab && pParse->pToplevel );
  assert( pStepList );
  assert( v!=0 );
  for(pStep=pStepList; pStep; pStep=pStep->pNext){
     
    pParse->eOrconf = (orconf==OE_Default)?pStep->orconf:(u8)orconf;
    assert( pParse->okConstFactor==0 );

    switch( pStep->op ){
      case TK_UPDATE: {
        sqlite3Update(pParse, 
          targetSrcList(pParse, pStep),
          sqlite3ExprListDup(db, pStep->pExprList, 0), 
          sqlite3ExprDup(db, pStep->pWhere, 0), 
          pParse->eOrconf
        );
        break;
      }
      case TK_INSERT: {
        sqlite3Insert(pParse, 
          targetSrcList(pParse, pStep),
          sqlite3SelectDup(db, pStep->pSelect, 0), 
          sqlite3IdListDup(db, pStep->pIdList), 
          pParse->eOrconf
        );
        break;
      }
      case TK_DELETE: {
        sqlite3DeleteFrom(pParse, 
          targetSrcList(pParse, pStep),
          sqlite3ExprDup(db, pStep->pWhere, 0)
        );
        break;
      }
      default: assert( pStep->op==TK_SELECT ); {
        SelectDest sDest;
        Select *pSelect = sqlite3SelectDup(db, pStep->pSelect, 0);
        sqlite3SelectDestInit(&sDest, SRT_Discard, 0);
        sqlite3Select(pParse, pSelect, &sDest);
        sqlite3SelectDelete(db, pSelect);
        break;
      }
    } 
    if( pStep->op!=TK_SELECT ){
      sqlite3VdbeAddOp0(v, OP_ResetCount);
    }
  }

  return 0;
}
