void WebGL2RenderingContextBase::UnregisterGetBufferSubDataAsyncCallback(
    WebGLGetBufferSubDataAsyncCallback* callback) {
  get_buffer_sub_data_async_callbacks_.erase(callback);
}
