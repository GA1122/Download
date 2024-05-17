 void AppCacheDatabase::ReadNamespaceRecords(
      sql::Statement* statement,
      NamespaceRecordVector* intercepts,
      NamespaceRecordVector* fallbacks) {
  while (statement->Step()) {
    AppCacheNamespaceType type = static_cast<AppCacheNamespaceType>(
        statement->ColumnInt(2));
    NamespaceRecordVector* records =
        (type == APPCACHE_FALLBACK_NAMESPACE) ? fallbacks : intercepts;
    records->push_back(NamespaceRecord());
    ReadNamespaceRecord(statement, &records->back());
  }
}
