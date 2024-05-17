static void addToVTrans(sqlite3 *db, VTable *pVTab){
   
  db->aVTrans[db->nVTrans++] = pVTab;
  sqlite3VtabLock(pVTab);
}
