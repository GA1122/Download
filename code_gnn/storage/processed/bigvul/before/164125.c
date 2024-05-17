void AppCacheDatabase::OnDatabaseError(int err, sql::Statement* stmt) {
  was_corruption_detected_ |= sql::IsErrorCatastrophic(err);
  if (!db_->IsExpectedSqliteError(err))
    DLOG(ERROR) << db_->GetErrorMessage();
}
