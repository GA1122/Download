int _stat(const char *pathname, struct stat *st) {
  int res = -1;
  bool is_sl;
  const char *fname = drop_dir(pathname, &is_sl);
  memset(st, 0, sizeof(*st));
   
  if (fname[0] == '\0' || strcmp(fname, ".") == 0) {
    st->st_ino = 0;
    st->st_mode = S_IFDIR | 0777;
    st->st_nlink = 1;
    st->st_size = 0;
    return 0;
  }
  if (is_sl) {
#ifdef MG_FS_SLFS
    res = fs_slfs_stat(fname, st);
#endif
  } else {
#ifdef CC3200_FS_SPIFFS
    res = fs_spiffs_stat(fname, st);
#endif
  }
  LOG(LL_DEBUG, ("stat(%s) = %d; fname = %s", pathname, res, fname));
  return res;
}