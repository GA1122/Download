bool DirectoryBackingStore::MigrateVersion75To76() {
  SetVersion(76);
  needs_column_refresh_ = true;
   return true;
 }
