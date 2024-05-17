int DirectoryBackingStore::GetVersion() {
  if (!sqlite_utils::DoesSqliteTableExist(load_dbhandle_, "share_version"))
    return 0;
  sqlite_utils::SQLStatement version_query;
  version_query.prepare(load_dbhandle_, "SELECT data from share_version");
  if (SQLITE_ROW != version_query.step())
    return 0;
  int value = version_query.column_int(0);
  if (version_query.reset() != SQLITE_OK)
    return 0;
  return value;
}
