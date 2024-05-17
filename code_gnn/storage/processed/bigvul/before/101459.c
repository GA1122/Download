bool DirectoryBackingStore::LoadInfo(Directory::KernelLoadInfo* info) {
  {
    sqlite_utils::SQLStatement query;
    query.prepare(load_dbhandle_,
                  "SELECT store_birthday, next_id, cache_guid, "
                  "notification_state "
                  "FROM share_info");
    if (SQLITE_ROW != query.step())
      return false;
    info->kernel_info.store_birthday = query.column_string(0);
    info->kernel_info.next_id = query.column_int64(1);
    info->cache_guid = query.column_string(2);
    query.column_blob_as_string(3, &info->kernel_info.notification_state);
  }
  {
    sqlite_utils::SQLStatement query;
    query.prepare(load_dbhandle_,
        "SELECT model_id, progress_marker, initial_sync_ended "
        "FROM models");
    while (SQLITE_ROW == query.step()) {
      ModelType type = ModelIdToModelTypeEnum(query.column_blob(0),
                                              query.column_bytes(0));
      if (type != UNSPECIFIED && type != TOP_LEVEL_FOLDER) {
        info->kernel_info.download_progress[type].ParseFromArray(
            query.column_blob(1), query.column_bytes(1));
        info->kernel_info.initial_sync_ended[type] = query.column_bool(2);
      }
    }
  }
  {
    sqlite_utils::SQLStatement query;
    query.prepare(load_dbhandle_,
                  "SELECT MAX(metahandle) FROM metas");
    if (SQLITE_ROW != query.step())
      return false;
    info->max_metahandle = query.column_int64(0);
  }
  return true;
}
