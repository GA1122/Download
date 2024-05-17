int SQLiteDatabase::RunVacuumCommand() {
  if (!ExecuteCommand("VACUUM;"))
    SQL_DVLOG(1) << "Unable to vacuum database -" << LastErrorMsg();
  return LastError();
}
