bool DirectoryBackingStore::MigrateVersion70To71() {
  if (SQLITE_DONE != CreateV71ModelsTable())
    return false;

  {
    sqlite_utils::SQLStatement fetch;
    fetch.prepare(load_dbhandle_,
        "SELECT last_sync_timestamp, initial_sync_ended FROM share_info");

    if (SQLITE_ROW != fetch.step())
      return false;
    int64 last_sync_timestamp = fetch.column_int64(0);
    bool initial_sync_ended = fetch.column_bool(1);
    if (SQLITE_DONE != fetch.step())
      return false;
    sqlite_utils::SQLStatement update;
    update.prepare(load_dbhandle_, "INSERT INTO models (model_id, "
        "last_download_timestamp, initial_sync_ended) VALUES (?, ?, ?)");
    string bookmark_model_id = ModelTypeEnumToModelId(BOOKMARKS);
    update.bind_blob(0, bookmark_model_id.data(), bookmark_model_id.size());
    update.bind_int64(1, last_sync_timestamp);
    update.bind_bool(2, initial_sync_ended);
    if (SQLITE_DONE != update.step())
      return false;
  }

  const bool kCreateAsTempShareInfo = true;

  int result =
      CreateShareInfoTableVersion71(kCreateAsTempShareInfo);
  if (result != SQLITE_DONE)
    return false;
  result = ExecQuery(load_dbhandle_,
                     "INSERT INTO temp_share_info (id, name, store_birthday, "
                     "db_create_version, db_create_time, next_id, cache_guid) "
                     "SELECT id, name, store_birthday, db_create_version, "
                     "db_create_time, next_id, cache_guid FROM share_info");
  if (result != SQLITE_DONE)
    return false;
  SafeDropTable("share_info");
  result = ExecQuery(load_dbhandle_,
      "ALTER TABLE temp_share_info RENAME TO share_info");
  if (result != SQLITE_DONE)
    return false;
  SetVersion(71);
  return true;
}
