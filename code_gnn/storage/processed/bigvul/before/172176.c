eager_reader_t *eager_reader_new(
 int fd_to_read,
 const allocator_t *allocator,
 size_t buffer_size,
 size_t max_buffer_count,
 const char *thread_name) {

  assert(fd_to_read != INVALID_FD);
  assert(allocator != NULL);
  assert(buffer_size > 0);
  assert(max_buffer_count > 0);
  assert(thread_name != NULL && *thread_name != '\0');

 eager_reader_t *ret = osi_calloc(sizeof(eager_reader_t));
 if (!ret) {
    LOG_ERROR("%s unable to allocate memory for new eager_reader.", __func__);
 goto error;
 }

  ret->allocator = allocator;
  ret->inbound_fd = fd_to_read;

  ret->bytes_available_fd = eventfd(0, 0);
 if (ret->bytes_available_fd == INVALID_FD) {
    LOG_ERROR("%s unable to create output reading semaphore.", __func__);
 goto error;
 }

  ret->buffer_size = buffer_size;

  ret->buffers = fixed_queue_new(max_buffer_count);
 if (!ret->buffers) {
    LOG_ERROR("%s unable to create buffers queue.", __func__);
 goto error;
 }

  ret->inbound_read_thread = thread_new(thread_name);
 if (!ret->inbound_read_thread) {
    LOG_ERROR("%s unable to make reading thread.", __func__);
 goto error;
 }

  ret->inbound_read_object = reactor_register(
    thread_get_reactor(ret->inbound_read_thread),
    fd_to_read,
    ret,
    inbound_data_waiting,
    NULL
 );

 return ret;

error:;
  eager_reader_free(ret);
 return NULL;
}
