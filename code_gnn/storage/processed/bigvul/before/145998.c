void WebGL2RenderingContextBase::DestroyContext() {
  for (auto& callback : get_buffer_sub_data_async_callbacks_) {
    callback->Destroy();
  }
  get_buffer_sub_data_async_callbacks_.clear();

  WebGLRenderingContextBase::DestroyContext();
}
