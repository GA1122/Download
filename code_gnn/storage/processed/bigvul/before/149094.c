static TriggerPrg *codeRowTrigger(
  Parse *pParse,        
  Trigger *pTrigger,    
  Table *pTab,          
  int orconf            
){
  Parse *pTop = sqlite3ParseToplevel(pParse);
  sqlite3 *db = pParse->db;    
  TriggerPrg *pPrg;            
  Expr *pWhen = 0;             
  Vdbe *v;                     
  NameContext sNC;             
  SubProgram *pProgram = 0;    
  Parse *pSubParse;            
  int iEndTrigger = 0;         

  assert( pTrigger->zName==0 || pTab==tableOfTrigger(pTrigger) );
  assert( pTop->pVdbe );

   
  pPrg = sqlite3DbMallocZero(db, sizeof(TriggerPrg));
  if( !pPrg ) return 0;
  pPrg->pNext = pTop->pTriggerPrg;
  pTop->pTriggerPrg = pPrg;
  pPrg->pProgram = pProgram = sqlite3DbMallocZero(db, sizeof(SubProgram));
  if( !pProgram ) return 0;
  sqlite3VdbeLinkSubProgram(pTop->pVdbe, pProgram);
  pPrg->pTrigger = pTrigger;
  pPrg->orconf = orconf;
  pPrg->aColmask[0] = 0xffffffff;
  pPrg->aColmask[1] = 0xffffffff;

   
  pSubParse = sqlite3StackAllocZero(db, sizeof(Parse));
  if( !pSubParse ) return 0;
  memset(&sNC, 0, sizeof(sNC));
  sNC.pParse = pSubParse;
  pSubParse->db = db;
  pSubParse->pTriggerTab = pTab;
  pSubParse->pToplevel = pTop;
  pSubParse->zAuthContext = pTrigger->zName;
  pSubParse->eTriggerOp = pTrigger->op;
  pSubParse->nQueryLoop = pParse->nQueryLoop;

  v = sqlite3GetVdbe(pSubParse);
  if( v ){
    VdbeComment((v, "Start: %s.%s (%s %s%s%s ON %s)", 
      pTrigger->zName, onErrorText(orconf),
      (pTrigger->tr_tm==TRIGGER_BEFORE ? "BEFORE" : "AFTER"),
        (pTrigger->op==TK_UPDATE ? "UPDATE" : ""),
        (pTrigger->op==TK_INSERT ? "INSERT" : ""),
        (pTrigger->op==TK_DELETE ? "DELETE" : ""),
      pTab->zName
    ));
#ifndef SQLITE_OMIT_TRACE
    sqlite3VdbeChangeP4(v, -1, 
      sqlite3MPrintf(db, "-- TRIGGER %s", pTrigger->zName), P4_DYNAMIC
    );
#endif

     
    if( pTrigger->pWhen ){
      pWhen = sqlite3ExprDup(db, pTrigger->pWhen, 0);
      if( SQLITE_OK==sqlite3ResolveExprNames(&sNC, pWhen) 
       && db->mallocFailed==0 
      ){
        iEndTrigger = sqlite3VdbeMakeLabel(v);
        sqlite3ExprIfFalse(pSubParse, pWhen, iEndTrigger, SQLITE_JUMPIFNULL);
      }
      sqlite3ExprDelete(db, pWhen);
    }

     
    codeTriggerProgram(pSubParse, pTrigger->step_list, orconf);

     
    if( iEndTrigger ){
      sqlite3VdbeResolveLabel(v, iEndTrigger);
    }
    sqlite3VdbeAddOp0(v, OP_Halt);
    VdbeComment((v, "End: %s.%s", pTrigger->zName, onErrorText(orconf)));

    transferParseError(pParse, pSubParse);
    if( db->mallocFailed==0 ){
      pProgram->aOp = sqlite3VdbeTakeOpArray(v, &pProgram->nOp, &pTop->nMaxArg);
    }
    pProgram->nMem = pSubParse->nMem;
    pProgram->nCsr = pSubParse->nTab;
    pProgram->token = (void *)pTrigger;
    pPrg->aColmask[0] = pSubParse->oldmask;
    pPrg->aColmask[1] = pSubParse->newmask;
    sqlite3VdbeDelete(v);
  }

  assert( !pSubParse->pAinc       && !pSubParse->pZombieTab );
  assert( !pSubParse->pTriggerPrg && !pSubParse->nMaxArg );
  sqlite3ParserReset(pSubParse);
  sqlite3StackFree(db, pSubParse);

  return pPrg;
}
