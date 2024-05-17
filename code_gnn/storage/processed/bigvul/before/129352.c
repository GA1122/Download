GLES2DecoderImpl::GLES2DecoderImpl(ContextGroup* group)
    : GLES2Decoder(),
      group_(group),
      logger_(&debug_marker_manager_),
      state_(group_->feature_info(), this, &logger_),
      unpack_flip_y_(false),
      unpack_premultiply_alpha_(false),
      unpack_unpremultiply_alpha_(false),
      attrib_0_buffer_id_(0),
      attrib_0_buffer_matches_value_(true),
      attrib_0_size_(0),
      fixed_attrib_buffer_id_(0),
      fixed_attrib_buffer_size_(0),
      offscreen_target_color_format_(0),
      offscreen_target_depth_format_(0),
      offscreen_target_stencil_format_(0),
      offscreen_target_samples_(0),
      offscreen_target_buffer_preserved_(true),
      offscreen_saved_color_format_(0),
      back_buffer_color_format_(0),
      back_buffer_has_depth_(false),
      back_buffer_has_stencil_(false),
      backbuffer_needs_clear_bits_(0),
      current_decoder_error_(error::kNoError),
      use_shader_translator_(true),
      validators_(group_->feature_info()->validators()),
      feature_info_(group_->feature_info()),
      frame_number_(0),
      has_robustness_extension_(false),
      reset_status_(GL_NO_ERROR),
      reset_by_robustness_extension_(false),
      supports_post_sub_buffer_(false),
      force_webgl_glsl_validation_(false),
      derivatives_explicitly_enabled_(false),
      frag_depth_explicitly_enabled_(false),
      draw_buffers_explicitly_enabled_(false),
      shader_texture_lod_explicitly_enabled_(false),
      compile_shader_always_succeeds_(false),
      lose_context_when_out_of_memory_(false),
      service_logging_(CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kEnableGPUServiceLoggingGPU)),
      viewport_max_width_(0),
      viewport_max_height_(0),
      texture_state_(group_->feature_info()
                         ->workarounds()
                         .texsubimage2d_faster_than_teximage2d),
      validation_texture_(0),
      validation_fbo_multisample_(0),
      validation_fbo_(0) {
  DCHECK(group);

  attrib_0_value_.v[0] = 0.0f;
  attrib_0_value_.v[1] = 0.0f;
  attrib_0_value_.v[2] = 0.0f;
  attrib_0_value_.v[3] = 1.0f;

  if (gfx::GetGLImplementation() == gfx::kGLImplementationMockGL ||
      CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisableGLSLTranslator)) {
    use_shader_translator_ = false;
  }
}
