void SQLiteDatabase::SetAuthorizer(DatabaseAuthorizer* auth) {
  if (!db_) {
    NOTREACHED() << "Attempt to set an authorizer on a non-open SQL database";
    return;
  }

  MutexLocker locker(authorizer_lock_);

  authorizer_ = auth;

  EnableAuthorizer(true);
}
