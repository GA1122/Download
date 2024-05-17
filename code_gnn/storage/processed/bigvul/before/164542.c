static void codeTableLocks(Parse *pParse){
  int i;
  Vdbe *pVdbe;

  pVdbe = sqlite3GetVdbe(pParse);
  assert( pVdbe!=0 );  

  for(i=0; i<pParse->nTableLock; i++){
    TableLock *p = &pParse->aTableLock[i];
    int p1 = p->iDb;
    sqlite3VdbeAddOp4(pVdbe, OP_TableLock, p1, p->iTab, p->isWriteLock,
                      p->zLockName, P4_STATIC);
  }
}