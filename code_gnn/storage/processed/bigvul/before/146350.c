void WebGLRenderingContextBase::RemoveBoundBuffer(WebGLBuffer* buffer) {
  if (bound_array_buffer_ == buffer)
    bound_array_buffer_ = nullptr;

  bound_vertex_array_object_->UnbindBuffer(buffer);
}
