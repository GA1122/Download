bool SQLiteDatabase::ExecuteCommand(const String& sql) {
  return SQLiteStatement(*this, sql).ExecuteCommand();
}
