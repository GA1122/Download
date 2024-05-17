bool DirectoryBackingStore::MigrateVersion69To70() {
  SetVersion(70);
  if (!AddColumn(&g_metas_columns[UNIQUE_SERVER_TAG])) {
    return false;
  }
  if (!AddColumn(&g_metas_columns[UNIQUE_CLIENT_TAG])) {
    return false;
  }
  needs_column_refresh_ = true;

  sqlite_utils::SQLStatement statement;
  statement.prepare(load_dbhandle_,
      "UPDATE metas SET unique_server_tag = singleton_tag");
  return statement.step() == SQLITE_DONE;
}
