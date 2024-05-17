static char* find_and_open_tombstone(int* fd) {
 char path[128];
 int oldest = -1;
 struct stat oldest_sb;
 for (int i = 0; i < MAX_TOMBSTONES; i++) {
    snprintf(path, sizeof(path), TOMBSTONE_TEMPLATE, i);

 struct stat sb;
 if (!stat(path, &sb)) {
 if (oldest < 0 || sb.st_mtime < oldest_sb.st_mtime) {
        oldest = i;
        oldest_sb.st_mtime = sb.st_mtime;
 }
 continue;
 }
 if (errno != ENOENT)
 continue;

 *fd = open(path, O_CREAT | O_EXCL | O_WRONLY | O_NOFOLLOW | O_CLOEXEC, 0600);
 if (*fd < 0)
 continue;  

    fchown(*fd, AID_SYSTEM, AID_SYSTEM);
 return strdup(path);
 }

 if (oldest < 0) {
    ALOGE("Failed to find a valid tombstone, default to using tombstone 0.\n");
    oldest = 0;
 }

  snprintf(path, sizeof(path), TOMBSTONE_TEMPLATE, oldest);
 *fd = open(path, O_CREAT | O_TRUNC | O_WRONLY | O_NOFOLLOW | O_CLOEXEC, 0600);
 if (*fd < 0) {
    ALOGE("failed to open tombstone file '%s': %s\n", path, strerror(errno));
 return NULL;
 }
  fchown(*fd, AID_SYSTEM, AID_SYSTEM);
 return strdup(path);
}
