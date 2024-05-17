bool DirectoryBackingStore::MigrateVersion68To69() {

  if (!AddColumn(&g_metas_columns[SPECIFICS]))
    return false;
  if (!AddColumn(&g_metas_columns[SERVER_SPECIFICS]))
    return false;

  if (!MigrateToSpecifics(("is_bookmark_object, bookmark_url, "
                           "bookmark_favicon, is_dir"),
                          "specifics",
                          &EncodeBookmarkURLAndFavicon)) {
    return false;
  }
  if (!MigrateToSpecifics(("server_is_bookmark_object, "
                           "server_bookmark_url, "
                           "server_bookmark_favicon, "
                           "server_is_dir"),
                          "server_specifics",
                          &EncodeBookmarkURLAndFavicon)) {
    return false;
  }

  sqlite_utils::SQLStatement clear_permanent_items;
  clear_permanent_items.prepare(load_dbhandle_,
      "UPDATE metas SET specifics = NULL, server_specifics = NULL WHERE "
      "singleton_tag IN ('google_chrome')");
  if (clear_permanent_items.step() != SQLITE_DONE)
    return false;

  SetVersion(69);
  needs_column_refresh_ = true;   
  return true;
}
