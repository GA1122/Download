void AppCacheDatabase::Disable() {
  VLOG(1) << "Disabling appcache database.";
  is_disabled_ = true;
  ResetConnectionAndTables();
 }
