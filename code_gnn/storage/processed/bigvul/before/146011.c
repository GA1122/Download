void WebGL2RenderingContextBase::RegisterGetBufferSubDataAsyncCallback(
    WebGLGetBufferSubDataAsyncCallback* callback) {
  get_buffer_sub_data_async_callbacks_.insert(callback);
}
