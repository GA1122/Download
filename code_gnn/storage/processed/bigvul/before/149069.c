void chromium_sqlite3_initialize_win_sqlite3_file(sqlite3_file* file, HANDLE handle) {
  winFile* winSQLite3File = (winFile*)file;
  memset(file, 0, sizeof(*file));
  winSQLite3File->pMethod = &winIoMethod;
  winSQLite3File->h = handle;
}
