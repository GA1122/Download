void IndexedDBDatabase::VersionChangeAbortOperation(int64_t previous_version) {
  IDB_TRACE("IndexedDBDatabase::VersionChangeAbortOperation");
  metadata_.version = previous_version;
}
