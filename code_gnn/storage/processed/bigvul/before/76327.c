static int ensure_filepath(char *path) {
  struct stat st_buf;
  char *p;
  int ok;

  for (p = &path[1]; *p; p++) {
    if (*p != '/') continue;
    *p = '\0';
    ok = (stat(path, &st_buf) == 0) && S_ISDIR(st_buf.st_mode);
    if (!ok) ok = (mkdir(path, 0777 & ~user_umask) == 0);
    *p = '/';
    if (!ok) return 0;
  }
   return 1;
 }