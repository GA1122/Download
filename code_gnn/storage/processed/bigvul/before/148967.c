static SQLITE_NOINLINE Vdbe *allocVdbe(Parse *pParse){
  Vdbe *v = pParse->pVdbe = sqlite3VdbeCreate(pParse);
  if( v ) sqlite3VdbeAddOp2(v, OP_Init, 0, 1);
  if( pParse->pToplevel==0
   && OptimizationEnabled(pParse->db,SQLITE_FactorOutConst)
  ){
    pParse->okConstFactor = 1;
  }
  return v;
}
