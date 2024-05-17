static void corruptSchema(
  InitData *pData,      
  const char *zObj,     
  const char *zExtra    
){
  sqlite3 *db = pData->db;
  if( !db->mallocFailed && (db->flags & SQLITE_RecoveryMode)==0 ){
    char *z;
    if( zObj==0 ) zObj = "?";
    z = sqlite3MPrintf(db, "malformed database schema (%s)", zObj);
    if( zExtra ) z = sqlite3MPrintf(db, "%z - %s", z, zExtra);
    sqlite3DbFree(db, *pData->pzErrMsg);
    *pData->pzErrMsg = z;
  }
  pData->rc = db->mallocFailed ? SQLITE_NOMEM_BKPT : SQLITE_CORRUPT_BKPT;
}
