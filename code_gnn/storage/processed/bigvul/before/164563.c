static void cume_distStepFunc(
  sqlite3_context *pCtx,
  int nArg,
  sqlite3_value **apArg
){
  struct CallCount *p;
  assert( nArg==1 ); UNUSED_PARAMETER(nArg);

  p = (struct CallCount*)sqlite3_aggregate_context(pCtx, sizeof(*p));
  if( p ){
    if( p->nTotal==0 ){
      p->nTotal = sqlite3_value_int64(apArg[0]);
    }
    p->nStep++;
  }
}
