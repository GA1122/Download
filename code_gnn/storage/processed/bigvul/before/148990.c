static int autoIncBegin(
  Parse *pParse,       
  int iDb,             
  Table *pTab          
){
  int memId = 0;       
  if( (pTab->tabFlags & TF_Autoincrement)!=0
   && (pParse->db->flags & SQLITE_Vacuum)==0
  ){
    Parse *pToplevel = sqlite3ParseToplevel(pParse);
    AutoincInfo *pInfo;

    pInfo = pToplevel->pAinc;
    while( pInfo && pInfo->pTab!=pTab ){ pInfo = pInfo->pNext; }
    if( pInfo==0 ){
      pInfo = sqlite3DbMallocRawNN(pParse->db, sizeof(*pInfo));
      if( pInfo==0 ) return 0;
      pInfo->pNext = pToplevel->pAinc;
      pToplevel->pAinc = pInfo;
      pInfo->pTab = pTab;
      pInfo->iDb = iDb;
      pToplevel->nMem++;                   
      pInfo->regCtr = ++pToplevel->nMem;   
      pToplevel->nMem++;                   
    }
    memId = pInfo->regCtr;
  }
  return memId;
}
