 void AppCacheDatabase::ResetConnectionAndTables() {
  meta_table_.reset();
  db_.reset();
}
