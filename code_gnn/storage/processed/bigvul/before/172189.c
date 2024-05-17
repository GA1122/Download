void semaphore_free(semaphore_t *semaphore) {
 if (!semaphore)
 return;

 if (semaphore->fd != INVALID_FD)
    close(semaphore->fd);
  osi_free(semaphore);
}
