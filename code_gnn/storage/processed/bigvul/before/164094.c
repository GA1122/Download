bool AppCacheDatabase::DeleteDeletableResponseIds(
    const std::vector<int64_t>& response_ids) {
  static const char kSql[] =
      "DELETE FROM DeletableResponseIds WHERE response_id = ?";
  return RunCachedStatementWithIds(SQL_FROM_HERE, kSql, response_ids);
}
