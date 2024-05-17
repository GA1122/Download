static bool is32bit(pid_t tid) {
 char* exeline;
 if (asprintf(&exeline, "/proc/%d/exe", tid) == -1) {
 return false;
 }
 int fd = TEMP_FAILURE_RETRY(open(exeline, O_RDONLY | O_CLOEXEC));
 int saved_errno = errno;
  free(exeline);
 if (fd == -1) {
    ALOGW("Failed to open /proc/%d/exe %s", tid, strerror(saved_errno));
 return false;
 }

 char ehdr[EI_NIDENT];
 ssize_t bytes = TEMP_FAILURE_RETRY(read(fd, &ehdr, sizeof(ehdr)));
  close(fd);
 if (bytes != (ssize_t) sizeof(ehdr) || memcmp(ELFMAG, ehdr, SELFMAG) != 0) {
 return false;
 }
 if (ehdr[EI_CLASS] == ELFCLASS32) {
 return true;
 }
 return false;
}
