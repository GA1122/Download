bool DirectoryBackingStore::OpenAndConfigureHandleHelper(
    sqlite3** handle) const {
  if (SQLITE_OK == sqlite_utils::OpenSqliteDb(backing_filepath_, handle)) {
    sqlite_utils::scoped_sqlite_db_ptr scoped_handle(*handle);
    sqlite3_busy_timeout(scoped_handle.get(), std::numeric_limits<int>::max());
    {
      string integrity_error;
      bool is_ok = CheckIntegrity(scoped_handle.get(), &integrity_error);
      if (!is_ok) {
        LOG(ERROR) << "Integrity check failed: " << integrity_error;
        return false;
      }
    }
    {
      sqlite_utils::SQLStatement statement;
      statement.prepare(scoped_handle.get(), "PRAGMA fullfsync = 1");
      if (SQLITE_DONE != statement.step()) {
        LOG(ERROR) << sqlite3_errmsg(scoped_handle.get());
        return false;
      }
    }
    {
      sqlite_utils::SQLStatement statement;
      statement.prepare(scoped_handle.get(), "PRAGMA synchronous = 2");
      if (SQLITE_DONE != statement.step()) {
        LOG(ERROR) << sqlite3_errmsg(scoped_handle.get());
        return false;
      }
    }
    sqlite3_busy_timeout(scoped_handle.release(),
                         kDirectoryBackingStoreBusyTimeoutMs);
#if defined(OS_WIN)
    const DWORD attrs = GetFileAttributes(backing_filepath_.value().c_str());
    const BOOL attrs_set =
      SetFileAttributes(backing_filepath_.value().c_str(),
                        attrs | FILE_ATTRIBUTE_NOT_CONTENT_INDEXED);
#endif

    return true;
  }
  return false;
}
