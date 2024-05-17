DirectoryBackingStore::~DirectoryBackingStore() {
  if (NULL != load_dbhandle_) {
    sqlite3_close(load_dbhandle_);
    load_dbhandle_ = NULL;
  }
  if (NULL != save_dbhandle_) {
    sqlite3_close(save_dbhandle_);
    save_dbhandle_ = NULL;
  }
}
