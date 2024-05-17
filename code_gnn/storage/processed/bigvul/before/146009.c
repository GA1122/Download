bool WebGL2RenderingContextBase::IsBufferBoundToNonTransformFeedback(
    WebGLBuffer* buffer) {
  DCHECK(buffer);

  if (bound_array_buffer_ == buffer ||
      bound_vertex_array_object_->BoundElementArrayBuffer() == buffer ||
      bound_copy_read_buffer_ == buffer || bound_copy_write_buffer_ == buffer ||
      bound_pixel_pack_buffer_ == buffer ||
      bound_pixel_unpack_buffer_ == buffer || bound_uniform_buffer_ == buffer) {
    return true;
  }

  for (size_t i = 0; i <= max_bound_uniform_buffer_index_; ++i) {
    if (bound_indexed_uniform_buffers_[i] == buffer)
      return true;
  }

  return false;
}
