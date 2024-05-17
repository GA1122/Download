sqlite3* DirectoryBackingStore::LazyGetSaveHandle() {
  if (!save_dbhandle_ && !OpenAndConfigureHandleHelper(&save_dbhandle_)) {
    NOTREACHED() << "Unable to open handle for saving";
    return NULL;
  }
  return save_dbhandle_;
}
