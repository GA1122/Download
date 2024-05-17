void SQLiteDatabase::EnableAuthorizer(bool enable) {
  if (authorizer_ && enable)
    sqlite3_set_authorizer(db_, SQLiteDatabase::AuthorizerFunction,
                           authorizer_.Get());
  else
    sqlite3_set_authorizer(db_, nullptr, nullptr);
}
