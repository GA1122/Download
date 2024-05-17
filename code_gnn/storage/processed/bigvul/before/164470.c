static void analyzeOneTable(
  Parse *pParse,    
  Table *pTab,      
  Index *pOnlyIdx,  
  int iStatCur,     
  int iMem,         
  int iTab          
){
  sqlite3 *db = pParse->db;     
  Index *pIdx;                  
  int iIdxCur;                  
  int iTabCur;                  
  Vdbe *v;                      
  int i;                        
  int jZeroRows = -1;           
  int iDb;                      
  u8 needTableCnt = 1;          
  int regNewRowid = iMem++;     
  int regStat4 = iMem++;        
  int regChng = iMem++;         
#ifdef SQLITE_ENABLE_STAT3_OR_STAT4
  int regRowid = iMem++;        
#endif
  int regTemp = iMem++;         
  int regTabname = iMem++;      
  int regIdxname = iMem++;      
  int regStat1 = iMem++;        
  int regPrev = iMem;           
#ifdef SQLITE_ENABLE_PREUPDATE_HOOK
  Table *pStat1 = 0;
#endif

  pParse->nMem = MAX(pParse->nMem, iMem);
  v = sqlite3GetVdbe(pParse);
  if( v==0 || NEVER(pTab==0) ){
    return;
  }
  if( pTab->tnum==0 ){
     
    return;
  }
  if( sqlite3_strlike("sqlite\\_%", pTab->zName, '\\')==0 ){
     
    return;
  }
  assert( sqlite3BtreeHoldsAllMutexes(db) );
  iDb = sqlite3SchemaToIndex(db, pTab->pSchema);
  assert( iDb>=0 );
  assert( sqlite3SchemaMutexHeld(db, iDb, 0) );
#ifndef SQLITE_OMIT_AUTHORIZATION
  if( sqlite3AuthCheck(pParse, SQLITE_ANALYZE, pTab->zName, 0,
      db->aDb[iDb].zDbSName ) ){
    return;
  }
#endif

#ifdef SQLITE_ENABLE_PREUPDATE_HOOK
  if( db->xPreUpdateCallback ){
    pStat1 = (Table*)sqlite3DbMallocZero(db, sizeof(Table) + 13);
    if( pStat1==0 ) return;
    pStat1->zName = (char*)&pStat1[1];
    memcpy(pStat1->zName, "sqlite_stat1", 13);
    pStat1->nCol = 3;
    pStat1->iPKey = -1;
    sqlite3VdbeAddOp4(pParse->pVdbe, OP_Noop, 0, 0, 0,(char*)pStat1,P4_DYNBLOB);
  }
#endif

   
  sqlite3TableLock(pParse, iDb, pTab->tnum, 0, pTab->zName);
  iTabCur = iTab++;
  iIdxCur = iTab++;
  pParse->nTab = MAX(pParse->nTab, iTab);
  sqlite3OpenTable(pParse, iTabCur, iDb, pTab, OP_OpenRead);
  sqlite3VdbeLoadString(v, regTabname, pTab->zName);

  for(pIdx=pTab->pIndex; pIdx; pIdx=pIdx->pNext){
    int nCol;                      
    int addrRewind;                
    int addrNextRow;               
    const char *zIdxName;          
    int nColTest;                  

    if( pOnlyIdx && pOnlyIdx!=pIdx ) continue;
    if( pIdx->pPartIdxWhere==0 ) needTableCnt = 0;
    if( !HasRowid(pTab) && IsPrimaryKeyIndex(pIdx) ){
      nCol = pIdx->nKeyCol;
      zIdxName = pTab->zName;
      nColTest = nCol - 1;
    }else{
      nCol = pIdx->nColumn;
      zIdxName = pIdx->zName;
      nColTest = pIdx->uniqNotNull ? pIdx->nKeyCol-1 : nCol-1;
    }

     
    sqlite3VdbeLoadString(v, regIdxname, zIdxName);
    VdbeComment((v, "Analysis for %s.%s", pTab->zName, zIdxName));

     

     
    pParse->nMem = MAX(pParse->nMem, regPrev+nColTest);

     
    assert( iDb==sqlite3SchemaToIndex(db, pIdx->pSchema) );
    sqlite3VdbeAddOp3(v, OP_OpenRead, iIdxCur, pIdx->tnum, iDb);
    sqlite3VdbeSetP4KeyInfo(pParse, pIdx);
    VdbeComment((v, "%s", pIdx->zName));

     
#ifdef SQLITE_ENABLE_STAT3_OR_STAT4
    sqlite3VdbeAddOp2(v, OP_Count, iIdxCur, regStat4+3);
#endif
    sqlite3VdbeAddOp2(v, OP_Integer, nCol, regStat4+1);
    sqlite3VdbeAddOp2(v, OP_Integer, pIdx->nKeyCol, regStat4+2);
    sqlite3VdbeAddOp4(v, OP_Function0, 0, regStat4+1, regStat4,
                     (char*)&statInitFuncdef, P4_FUNCDEF);
    sqlite3VdbeChangeP5(v, 2+IsStat34);

     
    addrRewind = sqlite3VdbeAddOp1(v, OP_Rewind, iIdxCur);
    VdbeCoverage(v);
    sqlite3VdbeAddOp2(v, OP_Integer, 0, regChng);
    addrNextRow = sqlite3VdbeCurrentAddr(v);

    if( nColTest>0 ){
      int endDistinctTest = sqlite3VdbeMakeLabel(pParse);
      int *aGotoChng;                
      aGotoChng = sqlite3DbMallocRawNN(db, sizeof(int)*nColTest);
      if( aGotoChng==0 ) continue;

       
      sqlite3VdbeAddOp0(v, OP_Goto);
      addrNextRow = sqlite3VdbeCurrentAddr(v);
      if( nColTest==1 && pIdx->nKeyCol==1 && IsUniqueIndex(pIdx) ){
         
        sqlite3VdbeAddOp2(v, OP_NotNull, regPrev, endDistinctTest);
        VdbeCoverage(v);
      }
      for(i=0; i<nColTest; i++){
        char *pColl = (char*)sqlite3LocateCollSeq(pParse, pIdx->azColl[i]);
        sqlite3VdbeAddOp2(v, OP_Integer, i, regChng);
        sqlite3VdbeAddOp3(v, OP_Column, iIdxCur, i, regTemp);
        aGotoChng[i] =
        sqlite3VdbeAddOp4(v, OP_Ne, regTemp, 0, regPrev+i, pColl, P4_COLLSEQ);
        sqlite3VdbeChangeP5(v, SQLITE_NULLEQ);
        VdbeCoverage(v);
      }
      sqlite3VdbeAddOp2(v, OP_Integer, nColTest, regChng);
      sqlite3VdbeGoto(v, endDistinctTest);


       
      sqlite3VdbeJumpHere(v, addrNextRow-1);
      for(i=0; i<nColTest; i++){
        sqlite3VdbeJumpHere(v, aGotoChng[i]);
        sqlite3VdbeAddOp3(v, OP_Column, iIdxCur, i, regPrev+i);
      }
      sqlite3VdbeResolveLabel(v, endDistinctTest);
      sqlite3DbFree(db, aGotoChng);
    }

     
#ifdef SQLITE_ENABLE_STAT3_OR_STAT4
    assert( regRowid==(regStat4+2) );
    if( HasRowid(pTab) ){
      sqlite3VdbeAddOp2(v, OP_IdxRowid, iIdxCur, regRowid);
    }else{
      Index *pPk = sqlite3PrimaryKeyIndex(pIdx->pTable);
      int j, k, regKey;
      regKey = sqlite3GetTempRange(pParse, pPk->nKeyCol);
      for(j=0; j<pPk->nKeyCol; j++){
        k = sqlite3ColumnOfIndex(pIdx, pPk->aiColumn[j]);
        assert( k>=0 && k<pIdx->nColumn );
        sqlite3VdbeAddOp3(v, OP_Column, iIdxCur, k, regKey+j);
        VdbeComment((v, "%s", pTab->aCol[pPk->aiColumn[j]].zName));
      }
      sqlite3VdbeAddOp3(v, OP_MakeRecord, regKey, pPk->nKeyCol, regRowid);
      sqlite3ReleaseTempRange(pParse, regKey, pPk->nKeyCol);
    }
#endif
    assert( regChng==(regStat4+1) );
    sqlite3VdbeAddOp4(v, OP_Function0, 1, regStat4, regTemp,
                     (char*)&statPushFuncdef, P4_FUNCDEF);
    sqlite3VdbeChangeP5(v, 2+IsStat34);
    sqlite3VdbeAddOp2(v, OP_Next, iIdxCur, addrNextRow); VdbeCoverage(v);

     
    callStatGet(v, regStat4, STAT_GET_STAT1, regStat1);
    assert( "BBB"[0]==SQLITE_AFF_TEXT );
    sqlite3VdbeAddOp4(v, OP_MakeRecord, regTabname, 3, regTemp, "BBB", 0);
    sqlite3VdbeAddOp2(v, OP_NewRowid, iStatCur, regNewRowid);
    sqlite3VdbeAddOp3(v, OP_Insert, iStatCur, regTemp, regNewRowid);
#ifdef SQLITE_ENABLE_PREUPDATE_HOOK
    sqlite3VdbeChangeP4(v, -1, (char*)pStat1, P4_TABLE);
#endif
    sqlite3VdbeChangeP5(v, OPFLAG_APPEND);

     
#ifdef SQLITE_ENABLE_STAT3_OR_STAT4
    {
      int regEq = regStat1;
      int regLt = regStat1+1;
      int regDLt = regStat1+2;
      int regSample = regStat1+3;
      int regCol = regStat1+4;
      int regSampleRowid = regCol + nCol;
      int addrNext;
      int addrIsNull;
      u8 seekOp = HasRowid(pTab) ? OP_NotExists : OP_NotFound;

      pParse->nMem = MAX(pParse->nMem, regCol+nCol);

      addrNext = sqlite3VdbeCurrentAddr(v);
      callStatGet(v, regStat4, STAT_GET_ROWID, regSampleRowid);
      addrIsNull = sqlite3VdbeAddOp1(v, OP_IsNull, regSampleRowid);
      VdbeCoverage(v);
      callStatGet(v, regStat4, STAT_GET_NEQ, regEq);
      callStatGet(v, regStat4, STAT_GET_NLT, regLt);
      callStatGet(v, regStat4, STAT_GET_NDLT, regDLt);
      sqlite3VdbeAddOp4Int(v, seekOp, iTabCur, addrNext, regSampleRowid, 0);
      VdbeCoverage(v);
#ifdef SQLITE_ENABLE_STAT3
      sqlite3ExprCodeLoadIndexColumn(pParse, pIdx, iTabCur, 0, regSample);
#else
      for(i=0; i<nCol; i++){
        sqlite3ExprCodeLoadIndexColumn(pParse, pIdx, iTabCur, i, regCol+i);
      }
      sqlite3VdbeAddOp3(v, OP_MakeRecord, regCol, nCol, regSample);
#endif
      sqlite3VdbeAddOp3(v, OP_MakeRecord, regTabname, 6, regTemp);
      sqlite3VdbeAddOp2(v, OP_NewRowid, iStatCur+1, regNewRowid);
      sqlite3VdbeAddOp3(v, OP_Insert, iStatCur+1, regTemp, regNewRowid);
      sqlite3VdbeAddOp2(v, OP_Goto, 1, addrNext);  
      sqlite3VdbeJumpHere(v, addrIsNull);
    }
#endif  

     
    sqlite3VdbeJumpHere(v, addrRewind);
  }


   
  if( pOnlyIdx==0 && needTableCnt ){
    VdbeComment((v, "%s", pTab->zName));
    sqlite3VdbeAddOp2(v, OP_Count, iTabCur, regStat1);
    jZeroRows = sqlite3VdbeAddOp1(v, OP_IfNot, regStat1); VdbeCoverage(v);
    sqlite3VdbeAddOp2(v, OP_Null, 0, regIdxname);
    assert( "BBB"[0]==SQLITE_AFF_TEXT );
    sqlite3VdbeAddOp4(v, OP_MakeRecord, regTabname, 3, regTemp, "BBB", 0);
    sqlite3VdbeAddOp2(v, OP_NewRowid, iStatCur, regNewRowid);
    sqlite3VdbeAddOp3(v, OP_Insert, iStatCur, regTemp, regNewRowid);
    sqlite3VdbeChangeP5(v, OPFLAG_APPEND);
#ifdef SQLITE_ENABLE_PREUPDATE_HOOK
    sqlite3VdbeChangeP4(v, -1, (char*)pStat1, P4_TABLE);
#endif
    sqlite3VdbeJumpHere(v, jZeroRows);
  }
}
