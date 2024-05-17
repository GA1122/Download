bool DirectoryBackingStore::MigrateVersion73To74() {

  int result =
      ExecQuery(load_dbhandle_,
                "ALTER TABLE share_info ADD COLUMN autofill_migration_state "
                "INT default 0");
  if (result != SQLITE_DONE)
    return false;

  result = ExecQuery(load_dbhandle_,
                "ALTER TABLE share_info ADD COLUMN "
                "bookmarks_added_during_autofill_migration "
                "INT default 0");

  if (result != SQLITE_DONE)
    return false;

  result = ExecQuery(load_dbhandle_,
                "ALTER TABLE share_info ADD COLUMN autofill_migration_time "
                "INT default 0");

  if (result != SQLITE_DONE)
    return false;

  result = ExecQuery(load_dbhandle_,
                "ALTER TABLE share_info ADD COLUMN "
                "autofill_entries_added_during_migration "
                "INT default 0");

  if (result != SQLITE_DONE)
    return false;

  result = ExecQuery(load_dbhandle_,
                "ALTER TABLE share_info ADD COLUMN "
                "autofill_profiles_added_during_migration "
                "INT default 0");

  if (result != SQLITE_DONE)
    return false;

  SetVersion(74);
  return true;
}
