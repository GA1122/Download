static SQLITE_NOINLINE int apiOomError(sqlite3 *db){
  sqlite3OomClear(db);
  sqlite3Error(db, SQLITE_NOMEM);
  return SQLITE_NOMEM_BKPT;
}
