void semaphore_wait(semaphore_t *semaphore) {
  assert(semaphore != NULL);
  assert(semaphore->fd != INVALID_FD);

 uint64_t value;
 if (eventfd_read(semaphore->fd, &value) == -1)
    LOG_ERROR("%s unable to wait on semaphore: %s", __func__, strerror(errno));
}
