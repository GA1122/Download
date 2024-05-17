bool DirectoryBackingStore::DropDeletedEntries() {
  static const char delete_metas[] = "DELETE FROM metas WHERE metahandle IN "
                                     "(SELECT metahandle from death_row)";
  sqlite_utils::SQLTransaction transaction(load_dbhandle_);
  transaction.Begin();
  if (SQLITE_DONE != ExecQuery(
                         load_dbhandle_,
                         "CREATE TEMP TABLE death_row (metahandle BIGINT)")) {
    return false;
  }
  if (SQLITE_DONE != ExecQuery(load_dbhandle_,
                               "INSERT INTO death_row "
                               "SELECT metahandle from metas WHERE is_del > 0 "
                               " AND is_unsynced < 1"
                               " AND is_unapplied_update < 1")) {
    return false;
  }
  if (SQLITE_DONE != ExecQuery(load_dbhandle_, delete_metas)) {
    return false;
  }
  if (SQLITE_DONE != ExecQuery(load_dbhandle_, "DROP TABLE death_row")) {
    return false;
  }
  transaction.Commit();
  return true;
}
