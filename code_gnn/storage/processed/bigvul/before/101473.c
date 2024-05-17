bool DirectoryBackingStore::RefreshColumns() {
  DCHECK(needs_column_refresh_);

  SafeDropTable("temp_metas");
  if (CreateMetasTable(true) != SQLITE_DONE)
    return false;

  std::string query = "INSERT INTO temp_metas (";
  AppendColumnList(&query);
  query.append(") SELECT ");
  AppendColumnList(&query);
  query.append(" FROM metas");
  if (ExecQuery(load_dbhandle_, query.c_str()) != SQLITE_DONE)
    return false;

  SafeDropTable("metas");

  int result = ExecQuery(load_dbhandle_,
                         "ALTER TABLE temp_metas RENAME TO metas");
  if (result != SQLITE_DONE)
    return false;

  SafeDropTable("temp_share_info");
  if (CreateShareInfoTable(true) != SQLITE_DONE)
    return false;

  result = ExecQuery(load_dbhandle_,
                "INSERT INTO temp_share_info (id, name, store_birthday, "
                "db_create_version, db_create_time, next_id, cache_guid,"
                "notification_state) "
                "SELECT id, name, store_birthday, db_create_version, "
                "db_create_time, next_id, cache_guid, notification_state "
                "FROM share_info");
  if (result != SQLITE_DONE)
    return false;

  SafeDropTable("share_info");
  result = ExecQuery(load_dbhandle_,
                     "ALTER TABLE temp_share_info RENAME TO share_info");
  if (result != SQLITE_DONE)
    return false;

  needs_column_refresh_ = false;
  return true;
}
