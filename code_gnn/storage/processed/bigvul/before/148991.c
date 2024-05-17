static void autoIncStep(Parse *pParse, int memId, int regRowid){
  if( memId>0 ){
    sqlite3VdbeAddOp2(pParse->pVdbe, OP_MemMax, memId, regRowid);
  }
}
