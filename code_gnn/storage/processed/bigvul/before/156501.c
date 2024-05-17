const char* SQLiteDatabase::LastErrorMsg() {
  if (db_)
    return sqlite3_errmsg(db_);
  return open_error_message_.IsNull() ? kNotOpenErrorMessage
                                      : open_error_message_.data();
}
