bool DirectoryBackingStore::MigrateVersion71To72() {
  SafeDropTable("extended_attributes");
  SetVersion(72);
  return true;
}
