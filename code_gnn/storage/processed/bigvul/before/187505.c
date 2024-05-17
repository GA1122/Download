 bool semaphore_try_wait(semaphore_t *semaphore) {
 
    assert(semaphore != NULL);
    assert(semaphore->fd != INVALID_FD);
  
  int flags = fcntl(semaphore->fd, F_GETFL);
//   int flags = TEMP_FAILURE_RETRY(fcntl(semaphore->fd, F_GETFL));
    if (flags == -1) {
      LOG_ERROR("%s unable to get flags for semaphore fd: %s", __func__, strerror(errno));
      return false;
    }
  if (fcntl(semaphore->fd, F_SETFL, flags | O_NONBLOCK) == -1) {
//   if (TEMP_FAILURE_RETRY(fcntl(semaphore->fd, F_SETFL, flags | O_NONBLOCK)) == -1) {
      LOG_ERROR("%s unable to set O_NONBLOCK for semaphore fd: %s", __func__, strerror(errno));
      return false;
    }
 
  eventfd_t value;
 
    if (eventfd_read(semaphore->fd, &value) == -1)
      return false;
  
  if (fcntl(semaphore->fd, F_SETFL, flags) == -1)
//   if (TEMP_FAILURE_RETRY(fcntl(semaphore->fd, F_SETFL, flags)) == -1)
      LOG_ERROR("%s unable to resetore flags for semaphore fd: %s", __func__, strerror(errno));
    return true;
  }