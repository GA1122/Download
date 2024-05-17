static int touch(char* path, mode_t mode) {
 int fd = open(path, O_RDWR | O_CREAT | O_EXCL | O_NOFOLLOW, mode);
 if (fd == -1) {
 if (errno == EEXIST) {
 return 0;
 } else {
            ERROR("Failed to open(%s): %s\n", path, strerror(errno));
 return -1;
 }
 }
    close(fd);
 return 0;
}
