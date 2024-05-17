static void corruptSchema(
  InitData *pData,      
  const char *zObj,     
  const char *zExtra    
){
  sqlite3 *db = pData->db;
  if( db->mallocFailed ){
    pData->rc = SQLITE_NOMEM_BKPT;
  }else if( pData->pzErrMsg[0]!=0 ){
     
  }else if( pData->mInitFlags & INITFLAG_AlterTable ){
    *pData->pzErrMsg = sqlite3DbStrDup(db, zExtra);
    pData->rc = SQLITE_ERROR;
  }else if( db->flags & SQLITE_WriteSchema ){
    pData->rc = SQLITE_CORRUPT_BKPT;
  }else{
    char *z;
    if( zObj==0 ) zObj = "?";
    z = sqlite3MPrintf(db, "malformed database schema (%s)", zObj);
    if( zExtra && zExtra[0] ) z = sqlite3MPrintf(db, "%z - %s", z, zExtra);
    *pData->pzErrMsg = z;
    pData->rc = SQLITE_CORRUPT_BKPT;
  }
}
