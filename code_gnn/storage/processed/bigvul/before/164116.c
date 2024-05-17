bool AppCacheDatabase::InsertDeletableResponseIds(
    const std::vector<int64_t>& response_ids) {
  static const char kSql[] =
      "INSERT INTO DeletableResponseIds (response_id) VALUES (?)";
  return RunCachedStatementWithIds(SQL_FROM_HERE, kSql, response_ids);
}
