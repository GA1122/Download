bool DirectoryBackingStore::MigrateVersion67To68() {
  SetVersion(68);
  needs_column_refresh_ = true;
  return true;
}
