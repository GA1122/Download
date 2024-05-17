__declspec(dllexport)
#endif
SQLITE_API int sqlite3_json_init(
  sqlite3 *db,
  char **pzErrMsg,
  const sqlite3_api_routines *pApi
){
  SQLITE_EXTENSION_INIT2(pApi);
  (void)pzErrMsg;   
  return sqlite3Json1Init(db);
}
