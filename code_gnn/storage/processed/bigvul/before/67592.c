static int robots_fsio_access(pr_fs_t *fs, const char *path, int mode,
    uid_t uid, gid_t gid, array_header *suppl_gids) {
  if (mode != R_OK) {
    errno = EACCES;
    return -1;
  }

  return 0;
}
