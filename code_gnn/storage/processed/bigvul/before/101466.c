bool DirectoryBackingStore::MigrateVersion72To73() {
  int result =
      ExecQuery(load_dbhandle_,
                "ALTER TABLE share_info ADD COLUMN notification_state BLOB");
  if (result != SQLITE_DONE)
    return false;
  SetVersion(73);
  return true;
}
