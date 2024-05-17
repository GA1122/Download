int DirectoryBackingStore::CreateV71ModelsTable() {
  return ExecQuery(load_dbhandle_,
      "CREATE TABLE models ("
      "model_id BLOB primary key, "
      "last_download_timestamp INT, "
      "initial_sync_ended BOOLEAN default 0)");
}
