void WebGL2RenderingContextBase::RemoveBoundBuffer(WebGLBuffer* buffer) {
  if (bound_copy_read_buffer_ == buffer)
    bound_copy_read_buffer_ = nullptr;
  if (bound_copy_write_buffer_ == buffer)
    bound_copy_write_buffer_ = nullptr;
  if (bound_pixel_pack_buffer_ == buffer)
    bound_pixel_pack_buffer_ = nullptr;
  if (bound_pixel_unpack_buffer_ == buffer)
    bound_pixel_unpack_buffer_ = nullptr;
  if (bound_transform_feedback_buffer_ == buffer)
    bound_transform_feedback_buffer_ = nullptr;
  if (bound_uniform_buffer_ == buffer)
    bound_uniform_buffer_ = nullptr;

  transform_feedback_binding_->UnbindBuffer(buffer);

  WebGLRenderingContextBase::RemoveBoundBuffer(buffer);
}
