int DirectoryBackingStore::CreateModelsTable() {
  return ExecQuery(load_dbhandle_,
      "CREATE TABLE models ("
      "model_id BLOB primary key, "
      "progress_marker BLOB, "
      "initial_sync_ended BOOLEAN default 0)");
}
