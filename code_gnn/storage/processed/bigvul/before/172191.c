semaphore_t *semaphore_new(unsigned int value) {
 semaphore_t *ret = osi_malloc(sizeof(semaphore_t));
 if (ret) {
    ret->fd = eventfd(value, EFD_SEMAPHORE);
 if (ret->fd == INVALID_FD) {
      LOG_ERROR("%s unable to allocate semaphore: %s", __func__, strerror(errno));
      osi_free(ret);
      ret = NULL;
 }
 }
 return ret;
}
