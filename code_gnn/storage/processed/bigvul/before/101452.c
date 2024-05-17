void DirectoryBackingStore::EndLoad() {
  sqlite3_close(load_dbhandle_);
  load_dbhandle_ = NULL;   
}
