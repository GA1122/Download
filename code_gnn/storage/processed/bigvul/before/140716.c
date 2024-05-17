void GLES2DecoderImpl::Destroy(bool have_context) {
  if (!initialized())
    return;

  DCHECK(!have_context || context_->IsCurrent(NULL));

  state_.vertex_attrib_manager = NULL;
  state_.default_vertex_attrib_manager = NULL;
  state_.texture_units.clear();
  state_.bound_array_buffer = NULL;
  state_.current_queries.clear();
  framebuffer_state_.bound_read_framebuffer = NULL;
  framebuffer_state_.bound_draw_framebuffer = NULL;
  state_.bound_renderbuffer = NULL;
  state_.bound_valuebuffer = NULL;

  if (offscreen_saved_color_texture_info_.get()) {
    DCHECK(offscreen_target_color_texture_);
    DCHECK_EQ(offscreen_saved_color_texture_info_->service_id(),
              offscreen_saved_color_texture_->id());
    offscreen_saved_color_texture_->Invalidate();
    offscreen_saved_color_texture_info_ = NULL;
  }
  if (have_context) {
    if (copy_texture_CHROMIUM_.get()) {
      copy_texture_CHROMIUM_->Destroy();
      copy_texture_CHROMIUM_.reset();
    }

    clear_framebuffer_blit_.reset();

    if (state_.current_program.get()) {
      program_manager()->UnuseProgram(shader_manager(),
                                      state_.current_program.get());
    }

    if (attrib_0_buffer_id_) {
      glDeleteBuffersARB(1, &attrib_0_buffer_id_);
    }
    if (fixed_attrib_buffer_id_) {
      glDeleteBuffersARB(1, &fixed_attrib_buffer_id_);
    }

    if (validation_texture_) {
      glDeleteTextures(1, &validation_texture_);
      glDeleteFramebuffersEXT(1, &validation_fbo_multisample_);
      glDeleteFramebuffersEXT(1, &validation_fbo_);
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
  }

  state_.current_program = NULL;

  copy_texture_CHROMIUM_.reset();
  clear_framebuffer_blit_.reset();

  if (query_manager_.get()) {
    query_manager_->Destroy(have_context);
    query_manager_.reset();
  }

  if (vertex_array_manager_ .get()) {
    vertex_array_manager_->Destroy(have_context);
    vertex_array_manager_.reset();
  }

  if (image_manager_.get()) {
    image_manager_->Destroy(have_context);
    image_manager_.reset();
  }

  offscreen_target_frame_buffer_.reset();
  offscreen_target_color_texture_.reset();
  offscreen_target_color_render_buffer_.reset();
  offscreen_target_depth_render_buffer_.reset();
  offscreen_target_stencil_render_buffer_.reset();
  offscreen_saved_frame_buffer_.reset();
  offscreen_saved_color_texture_.reset();
  offscreen_resolved_frame_buffer_.reset();
  offscreen_resolved_color_texture_.reset();

  fragment_translator_ = NULL;
  vertex_translator_ = NULL;

  async_pixel_transfer_manager_.reset();

  if (group_.get()) {
    framebuffer_manager()->RemoveObserver(this);
    group_->Destroy(this, have_context);
    group_ = NULL;
  }

  if (context_.get()) {
    context_->ReleaseCurrent(NULL);
    context_ = NULL;
  }

#if defined(OS_MACOSX)
  for (TextureToIOSurfaceMap::iterator it = texture_to_io_surface_map_.begin();
       it != texture_to_io_surface_map_.end(); ++it) {
    CFRelease(it->second);
  }
  texture_to_io_surface_map_.clear();
#endif
}
