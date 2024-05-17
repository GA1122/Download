int SQLiteDatabase::RunIncrementalVacuumCommand() {
  MutexLocker locker(authorizer_lock_);
  EnableAuthorizer(false);

  if (!ExecuteCommand("PRAGMA incremental_vacuum"))
    SQL_DVLOG(1) << "Unable to run incremental vacuum - " << LastErrorMsg();

  EnableAuthorizer(true);
  return LastError();
}
