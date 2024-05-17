static void callStatGet(Vdbe *v, int regStat4, int iParam, int regOut){
  assert( regOut!=regStat4 && regOut!=regStat4+1 );
#ifdef SQLITE_ENABLE_STAT3_OR_STAT4
  sqlite3VdbeAddOp2(v, OP_Integer, iParam, regStat4+1);
#elif SQLITE_DEBUG
  assert( iParam==STAT_GET_STAT1 );
#else
  UNUSED_PARAMETER( iParam );
#endif
  sqlite3VdbeAddOp4(v, OP_Function0, 0, regStat4, regOut,
                    (char*)&statGetFuncdef, P4_FUNCDEF);
  sqlite3VdbeChangeP5(v, 1 + IsStat34);
}
