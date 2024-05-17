bool DirectoryBackingStore::SetVersion(int version) {
  sqlite_utils::SQLStatement statement;
  statement.prepare(load_dbhandle_, "UPDATE share_version SET data = ?");
  statement.bind_int(0, version);
  return statement.step() == SQLITE_DONE;
}
