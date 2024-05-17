__declspec(dllexport)
#endif
SQLITE_API int sqlite3_stmt_init(
  sqlite3 *db,
  char **pzErrMsg,
  const sqlite3_api_routines *pApi
){
  int rc = SQLITE_OK;
  SQLITE_EXTENSION_INIT2(pApi);
#ifndef SQLITE_OMIT_VIRTUALTABLE
  rc = sqlite3StmtVtabInit(db);
#endif
  return rc;
}
