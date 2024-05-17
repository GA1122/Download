void GLES2DecoderImpl::Destroy(bool have_context) {
  if (!initialized())
    return;

  DCHECK(!have_context || context_->IsCurrent(nullptr));

  if (surface_)
    surface_->PrepareToDestroy(have_context);

  if (!have_context) {
    for (auto iter : texture_refs_pending_destruction_)
      iter->ForceContextLost();
  }
  texture_refs_pending_destruction_.clear();

  for (auto* iter : abstract_textures_)
    iter->OnDecoderWillDestroy(have_context);
  abstract_textures_.clear();

  ReleaseAllBackTextures(have_context);
  if (have_context) {
    if (apply_framebuffer_attachment_cmaa_intel_.get()) {
      apply_framebuffer_attachment_cmaa_intel_->Destroy();
      apply_framebuffer_attachment_cmaa_intel_.reset();
    }

    if (copy_tex_image_blit_.get()) {
      copy_tex_image_blit_->Destroy();
      copy_tex_image_blit_.reset();
    }

    if (copy_texture_chromium_.get()) {
      copy_texture_chromium_->Destroy();
      copy_texture_chromium_.reset();
    }

    if (srgb_converter_.get()) {
      srgb_converter_->Destroy();
      srgb_converter_.reset();
    }

    clear_framebuffer_blit_.reset();

    if (state_.current_program.get()) {
      program_manager()->UnuseProgram(shader_manager(),
                                      state_.current_program.get());
    }

    if (attrib_0_buffer_id_) {
      api()->glDeleteBuffersARBFn(1, &attrib_0_buffer_id_);
    }
    if (fixed_attrib_buffer_id_) {
      api()->glDeleteBuffersARBFn(1, &fixed_attrib_buffer_id_);
    }

    if (validation_fbo_) {
      api()->glDeleteFramebuffersEXTFn(1, &validation_fbo_multisample_);
      api()->glDeleteFramebuffersEXTFn(1, &validation_fbo_);
    }
    while (!validation_textures_.empty()) {
      GLuint tex;
      tex = validation_textures_.begin()->second;
      api()->glDeleteTexturesFn(1, &tex);
      validation_textures_.erase(validation_textures_.begin());
    }

    if (offscreen_target_frame_buffer_.get())
      offscreen_target_frame_buffer_->Destroy();
    if (offscreen_target_color_texture_.get())
      offscreen_target_color_texture_->Destroy();
    if (offscreen_target_color_render_buffer_.get())
      offscreen_target_color_render_buffer_->Destroy();
    if (offscreen_target_depth_render_buffer_.get())
      offscreen_target_depth_render_buffer_->Destroy();
    if (offscreen_target_stencil_render_buffer_.get())
      offscreen_target_stencil_render_buffer_->Destroy();
    if (offscreen_saved_frame_buffer_.get())
      offscreen_saved_frame_buffer_->Destroy();
    if (offscreen_saved_color_texture_.get())
      offscreen_saved_color_texture_->Destroy();
    if (offscreen_resolved_frame_buffer_.get())
      offscreen_resolved_frame_buffer_->Destroy();
    if (offscreen_resolved_color_texture_.get())
      offscreen_resolved_color_texture_->Destroy();
  } else {
    if (offscreen_target_frame_buffer_.get())
      offscreen_target_frame_buffer_->Invalidate();
    if (offscreen_target_color_texture_.get())
      offscreen_target_color_texture_->Invalidate();
    if (offscreen_target_color_render_buffer_.get())
      offscreen_target_color_render_buffer_->Invalidate();
    if (offscreen_target_depth_render_buffer_.get())
      offscreen_target_depth_render_buffer_->Invalidate();
    if (offscreen_target_stencil_render_buffer_.get())
      offscreen_target_stencil_render_buffer_->Invalidate();
    if (offscreen_saved_frame_buffer_.get())
      offscreen_saved_frame_buffer_->Invalidate();
    if (offscreen_saved_color_texture_.get())
      offscreen_saved_color_texture_->Invalidate();
    if (offscreen_resolved_frame_buffer_.get())
      offscreen_resolved_frame_buffer_->Invalidate();
    if (offscreen_resolved_color_texture_.get())
      offscreen_resolved_color_texture_->Invalidate();
    for (auto& fence : deschedule_until_finished_fences_) {
      fence->Invalidate();
    }

    if (group_ && group_->texture_manager())
      group_->texture_manager()->MarkContextLost();
    state_.MarkContextLost();
  }
  deschedule_until_finished_fences_.clear();

  ReportProgress();

  state_.vertex_attrib_manager = nullptr;
  state_.default_vertex_attrib_manager = nullptr;
  state_.texture_units.clear();
  state_.sampler_units.clear();
  state_.bound_array_buffer = nullptr;
  state_.bound_copy_read_buffer = nullptr;
  state_.bound_copy_write_buffer = nullptr;
  state_.bound_pixel_pack_buffer = nullptr;
  state_.bound_pixel_unpack_buffer = nullptr;
  state_.bound_transform_feedback_buffer = nullptr;
  state_.bound_uniform_buffer = nullptr;
  framebuffer_state_.bound_read_framebuffer = nullptr;
  framebuffer_state_.bound_draw_framebuffer = nullptr;
  state_.current_draw_framebuffer_client_id = 0;
  state_.bound_renderbuffer = nullptr;
  state_.bound_transform_feedback = nullptr;
  state_.default_transform_feedback = nullptr;
  state_.indexed_uniform_buffer_bindings = nullptr;

  state_.current_program = nullptr;

  apply_framebuffer_attachment_cmaa_intel_.reset();
  copy_tex_image_blit_.reset();
  copy_texture_chromium_.reset();
  srgb_converter_.reset();
  clear_framebuffer_blit_.reset();

  ReportProgress();

  if (framebuffer_manager_.get()) {
    framebuffer_manager_->Destroy(have_context);
    if (group_->texture_manager())
      group_->texture_manager()->RemoveFramebufferManager(
          framebuffer_manager_.get());
    framebuffer_manager_.reset();
  }

  multi_draw_manager_.reset();

  if (query_manager_.get()) {
    query_manager_->Destroy(have_context);
    query_manager_.reset();
  }

  if (gpu_fence_manager_.get()) {
    gpu_fence_manager_->Destroy(have_context);
    gpu_fence_manager_.reset();
  }

  if (vertex_array_manager_ .get()) {
    vertex_array_manager_->Destroy(have_context);
    vertex_array_manager_.reset();
  }

  if (transform_feedback_manager_.get()) {
    if (!have_context) {
      transform_feedback_manager_->MarkContextLost();
    }
    transform_feedback_manager_->Destroy();
    transform_feedback_manager_.reset();
  }

  ReportProgress();

  offscreen_target_frame_buffer_.reset();
  offscreen_target_color_texture_.reset();
  offscreen_target_color_render_buffer_.reset();
  offscreen_target_depth_render_buffer_.reset();
  offscreen_target_stencil_render_buffer_.reset();
  offscreen_saved_frame_buffer_.reset();
  offscreen_saved_color_texture_.reset();
  offscreen_resolved_frame_buffer_.reset();
  offscreen_resolved_color_texture_.reset();

  pending_readpixel_fences_ = base::queue<FenceCallback>();

  DestroyShaderTranslator();

  ReportProgress();

  if (gpu_tracer_) {
    gpu_tracer_->Destroy(have_context);
    gpu_tracer_.reset();
  }

  if (group_.get()) {
    group_->Destroy(this, have_context);
    group_ = nullptr;
  }

  if (context_.get()) {
    context_->ReleaseCurrent(nullptr);
    context_ = nullptr;
  }
  surface_ = nullptr;
}
