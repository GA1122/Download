DEFINE_TRACE_WRAPPERS(WebGL2RenderingContextBase) {
  visitor->TraceWrappers(read_framebuffer_binding_);
  visitor->TraceWrappers(transform_feedback_binding_);
  visitor->TraceWrappers(bound_copy_read_buffer_);
  visitor->TraceWrappers(bound_copy_write_buffer_);
  visitor->TraceWrappers(bound_pixel_pack_buffer_);
  visitor->TraceWrappers(bound_pixel_unpack_buffer_);
  visitor->TraceWrappers(bound_uniform_buffer_);
  for (auto& buf : bound_indexed_uniform_buffers_) {
    visitor->TraceWrappers(buf);
  }
  visitor->TraceWrappers(current_boolean_occlusion_query_);
  visitor->TraceWrappers(current_transform_feedback_primitives_written_query_);
  visitor->TraceWrappers(current_elapsed_query_);
  for (auto& unit : sampler_units_) {
    visitor->TraceWrappers(unit);
  }
  WebGLRenderingContextBase::TraceWrappers(visitor);
}
