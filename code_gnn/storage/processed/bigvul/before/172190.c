int semaphore_get_fd(const semaphore_t *semaphore) {
  assert(semaphore != NULL);
  assert(semaphore->fd != INVALID_FD);
 return semaphore->fd;
}
