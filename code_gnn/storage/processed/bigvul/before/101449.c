void DirectoryBackingStore::DropAllTables() {
  SafeDropTable("metas");
  SafeDropTable("temp_metas");
  SafeDropTable("share_info");
  SafeDropTable("temp_share_info");
  SafeDropTable("share_version");
  SafeDropTable("extended_attributes");
  SafeDropTable("models");
  SafeDropTable("temp_models");
  needs_column_refresh_ = false;
}
