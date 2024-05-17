bool AppCacheDatabase::InsertNamespaceRecords(
    const std::vector<NamespaceRecord>& records) {
  if (records.empty())
    return true;
  sql::Transaction transaction(db_.get());
  if (!transaction.Begin())
    return false;
  for (const auto& record : records) {
    if (!InsertNamespace(&record))
      return false;
  }
  return transaction.Commit();
}
