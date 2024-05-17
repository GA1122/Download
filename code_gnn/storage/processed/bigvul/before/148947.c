__declspec(dllexport)
#endif
SQLITE_API int sqlite3_fts_init(
  sqlite3 *db,
  char **pzErrMsg,
  const sqlite3_api_routines *pApi
){
  SQLITE_EXTENSION_INIT2(pApi);
  (void)pzErrMsg;   
  return fts5Init(db);
}
