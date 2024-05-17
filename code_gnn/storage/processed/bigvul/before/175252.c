int blockingWrite_helper(int fd, void *buffer, size_t len) {
 return android::blockingWrite(fd, buffer, len);
}
