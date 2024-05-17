bool AppCacheDatabase::InsertEntryRecords(
    const std::vector<EntryRecord>& records) {
  if (records.empty())
    return true;
  sql::Transaction transaction(db_.get());
  if (!transaction.Begin())
    return false;
  for (const auto& record : records) {
    if (!InsertEntry(&record))
      return false;
  }
  return transaction.Commit();
}
