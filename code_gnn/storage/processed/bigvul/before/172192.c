void semaphore_post(semaphore_t *semaphore) {
  assert(semaphore != NULL);
  assert(semaphore->fd != INVALID_FD);

 if (eventfd_write(semaphore->fd, 1ULL) == -1)
    LOG_ERROR("%s unable to post to semaphore: %s", __func__, strerror(errno));
}
