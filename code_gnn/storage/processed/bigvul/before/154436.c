void GLES2DecoderPassthroughImpl::Destroy(bool have_context) {
  if (have_context) {
    FlushErrors();
  }

  for (PendingReadPixels& pending_read_pixels : pending_read_pixels_) {
    if (have_context) {
      api()->glDeleteBuffersARBFn(1, &pending_read_pixels.buffer_service_id);
    } else {
      pending_read_pixels.fence->Invalidate();
    }
  }
  pending_read_pixels_.clear();

  for (auto& bound_texture_type : bound_textures_) {
    for (auto& bound_texture : bound_texture_type) {
      if (!have_context && bound_texture.texture) {
        bound_texture.texture->MarkContextLost();
      }
      bound_texture.texture = nullptr;
    }
  }

  if (resources_) {   
    for (PassthroughAbstractTextureImpl* iter : abstract_textures_) {
      resources_->textures_pending_destruction.insert(
          iter->OnDecoderWillDestroy());
    }
    abstract_textures_.clear();
    if (have_context) {
      resources_->DestroyPendingTextures( true);
    }
  }

  DeleteServiceObjects(&framebuffer_id_map_, have_context,
                       [this](GLuint client_id, GLuint framebuffer) {
                         api()->glDeleteFramebuffersEXTFn(1, &framebuffer);
                       });
  DeleteServiceObjects(&transform_feedback_id_map_, have_context,
                       [this](GLuint client_id, GLuint transform_feedback) {
                         api()->glDeleteTransformFeedbacksFn(
                             1, &transform_feedback);
                       });
  DeleteServiceObjects(&query_id_map_, have_context,
                       [this](GLuint client_id, GLuint query) {
                         api()->glDeleteQueriesFn(1, &query);
                       });
  DeleteServiceObjects(&vertex_array_id_map_, have_context,
                       [this](GLuint client_id, GLuint vertex_array) {
                         api()->glDeleteVertexArraysOESFn(1, &vertex_array);
                       });

  if (emulated_back_buffer_) {
    emulated_back_buffer_->Destroy(have_context);
    emulated_back_buffer_.reset();
  }

  if (emulated_front_buffer_) {
    emulated_front_buffer_->Destroy(have_context);
    emulated_front_buffer_.reset();
  }

  for (auto& in_use_color_texture : in_use_color_textures_) {
    in_use_color_texture->Destroy(have_context);
  }
  in_use_color_textures_.clear();

  for (auto& available_color_texture : available_color_textures_) {
    available_color_texture->Destroy(have_context);
  }
  available_color_textures_.clear();

  if (gpu_fence_manager_.get()) {
    gpu_fence_manager_->Destroy(have_context);
    gpu_fence_manager_.reset();
  }

  if (gpu_tracer_) {
    gpu_tracer_->Destroy(have_context);
    gpu_tracer_.reset();
  }

  if (multi_draw_manager_.get()) {
    multi_draw_manager_.reset();
  }

  if (!have_context) {
    for (auto& fence : deschedule_until_finished_fences_) {
      fence->Invalidate();
    }
  }
  deschedule_until_finished_fences_.clear();

  surface_ = nullptr;

  if (group_) {
    group_->Destroy(this, have_context);
    group_ = nullptr;
  }

  if (context_.get()) {
    context_->ReleaseCurrent(nullptr);
    context_ = nullptr;
  }
}
