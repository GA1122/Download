static int checkReadTransaction(sqlite3 *db, Btree *p){
  if( sqlite3BtreeIsInReadTrans(p) ){
    sqlite3ErrorWithMsg(db, SQLITE_ERROR, "destination database is in use");
    return SQLITE_ERROR;
  }
  return SQLITE_OK;
}
