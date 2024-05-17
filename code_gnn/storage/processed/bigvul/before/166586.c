void WebGL2RenderingContextBase::InitializeNewContext() {
  DCHECK(!isContextLost());
  DCHECK(GetDrawingBuffer());

  read_framebuffer_binding_ = nullptr;

  bound_copy_read_buffer_ = nullptr;
  bound_copy_write_buffer_ = nullptr;
  bound_pixel_pack_buffer_ = nullptr;
  bound_pixel_unpack_buffer_ = nullptr;
  bound_transform_feedback_buffer_ = nullptr;
  bound_uniform_buffer_ = nullptr;

  current_boolean_occlusion_query_ = nullptr;
  current_transform_feedback_primitives_written_query_ = nullptr;
  current_elapsed_query_ = nullptr;

  GLint num_combined_texture_image_units = 0;
  ContextGL()->GetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
                           &num_combined_texture_image_units);
  sampler_units_.clear();
  sampler_units_.resize(num_combined_texture_image_units);

  max_transform_feedback_separate_attribs_ = 0;
  ContextGL()->GetIntegerv(GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS,
                           &max_transform_feedback_separate_attribs_);
  default_transform_feedback_ = WebGLTransformFeedback::Create(
      this, WebGLTransformFeedback::TFTypeDefault);
  transform_feedback_binding_ = default_transform_feedback_;

  GLint max_uniform_buffer_bindings = 0;
  ContextGL()->GetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS,
                           &max_uniform_buffer_bindings);
  bound_indexed_uniform_buffers_.clear();
  bound_indexed_uniform_buffers_.resize(max_uniform_buffer_bindings);
  max_bound_uniform_buffer_index_ = 0;

  pack_row_length_ = 0;
  pack_skip_pixels_ = 0;
  pack_skip_rows_ = 0;
  unpack_row_length_ = 0;
  unpack_image_height_ = 0;
  unpack_skip_pixels_ = 0;
  unpack_skip_rows_ = 0;
  unpack_skip_images_ = 0;

  WebGLRenderingContextBase::InitializeNewContext();
}
