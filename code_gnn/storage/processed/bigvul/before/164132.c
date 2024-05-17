bool AppCacheDatabase::RunUniqueStatementWithInt64Result(const char* sql,
                                                         int64_t* result) {
  DCHECK(sql);
  sql::Statement statement(db_->GetUniqueStatement(sql));
  if (!statement.Step()) {
    return false;
  }
  *result = statement.ColumnInt64(0);
  return true;
}
