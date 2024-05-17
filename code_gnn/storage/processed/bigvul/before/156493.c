int SQLiteDatabase::AuthorizerFunction(void* user_data,
                                       int action_code,
                                       const char* parameter1,
                                       const char* parameter2,
                                       const char*  ,
                                       const char*  ) {
  DatabaseAuthorizer* auth = static_cast<DatabaseAuthorizer*>(user_data);
  DCHECK(auth);

  switch (action_code) {
    case SQLITE_CREATE_INDEX:
      return auth->CreateIndex(parameter1, parameter2);
    case SQLITE_CREATE_TABLE:
      return auth->CreateTable(parameter1);
    case SQLITE_CREATE_TEMP_INDEX:
      return auth->CreateTempIndex(parameter1, parameter2);
    case SQLITE_CREATE_TEMP_TABLE:
      return auth->CreateTempTable(parameter1);
    case SQLITE_CREATE_TEMP_TRIGGER:
      return auth->CreateTempTrigger(parameter1, parameter2);
    case SQLITE_CREATE_TEMP_VIEW:
      return auth->CreateTempView(parameter1);
    case SQLITE_CREATE_TRIGGER:
      return auth->CreateTrigger(parameter1, parameter2);
    case SQLITE_CREATE_VIEW:
      return auth->CreateView(parameter1);
    case SQLITE_DELETE:
      return auth->AllowDelete(parameter1);
    case SQLITE_DROP_INDEX:
      return auth->DropIndex(parameter1, parameter2);
    case SQLITE_DROP_TABLE:
      return auth->DropTable(parameter1);
    case SQLITE_DROP_TEMP_INDEX:
      return auth->DropTempIndex(parameter1, parameter2);
    case SQLITE_DROP_TEMP_TABLE:
      return auth->DropTempTable(parameter1);
    case SQLITE_DROP_TEMP_TRIGGER:
      return auth->DropTempTrigger(parameter1, parameter2);
    case SQLITE_DROP_TEMP_VIEW:
      return auth->DropTempView(parameter1);
    case SQLITE_DROP_TRIGGER:
      return auth->DropTrigger(parameter1, parameter2);
    case SQLITE_DROP_VIEW:
      return auth->DropView(parameter1);
    case SQLITE_INSERT:
      return auth->AllowInsert(parameter1);
    case SQLITE_PRAGMA:
      return auth->AllowPragma(parameter1, parameter2);
    case SQLITE_READ:
      return auth->AllowRead(parameter1, parameter2);
    case SQLITE_SELECT:
      return auth->AllowSelect();
    case SQLITE_TRANSACTION:
      return auth->AllowTransaction();
    case SQLITE_UPDATE:
      return auth->AllowUpdate(parameter1, parameter2);
    case SQLITE_ATTACH:
      return kSQLAuthDeny;
    case SQLITE_DETACH:
      return kSQLAuthDeny;
    case SQLITE_ALTER_TABLE:
      return auth->AllowAlterTable(parameter1, parameter2);
    case SQLITE_REINDEX:
      return auth->AllowReindex(parameter1);
    case SQLITE_ANALYZE:
      return auth->AllowAnalyze(parameter1);
    case SQLITE_CREATE_VTABLE:
      return auth->CreateVTable(parameter1, parameter2);
    case SQLITE_DROP_VTABLE:
      return auth->DropVTable(parameter1, parameter2);
    case SQLITE_FUNCTION:
      return auth->AllowFunction(parameter2);
    case SQLITE_SAVEPOINT:
      return kSQLAuthDeny;
    case SQLITE_RECURSIVE:
      return kSQLAuthDeny;
  }
  NOTREACHED();
  return kSQLAuthDeny;
}
