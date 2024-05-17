void uv__process_close(uv_process_t* handle) {
   
  QUEUE_REMOVE(&handle->queue);
  uv__handle_stop(handle);
}
