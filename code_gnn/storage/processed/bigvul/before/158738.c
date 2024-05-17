gpu::ContextResult GLES2DecoderImpl::Initialize(
    const scoped_refptr<gl::GLSurface>& surface,
    const scoped_refptr<gl::GLContext>& context,
    bool offscreen,
    const DisallowedFeatures& disallowed_features,
    const ContextCreationAttribs& attrib_helper) {
  TRACE_EVENT0("gpu", "GLES2DecoderImpl::Initialize");
  DCHECK(context->IsCurrent(surface.get()));
  DCHECK(!context_.get());
  state_.set_api(gl::g_current_gl_context);

  surfaceless_ = surface->IsSurfaceless() && !offscreen;

  set_initialized();
  gpu_state_tracer_ = GPUStateTracer::Create(&state_);

  if (group_->gpu_preferences().enable_gpu_debugging)
    set_debug(true);

  if (group_->gpu_preferences().enable_gpu_command_logging)
    SetLogCommands(true);

  compile_shader_always_succeeds_ =
      group_->gpu_preferences().compile_shader_always_succeeds;

  context_ = context;
  surface_ = surface;

  if (workarounds().rely_on_implicit_sync_for_swap_buffers)
    surface_->SetRelyOnImplicitSync();

  gpu_tracer_.reset(new GPUTracer(this));

  if (workarounds().disable_timestamp_queries) {
    GetGLContext()->CreateGPUTimingClient()->ForceTimeElapsedQuery();
  }

  lose_context_when_out_of_memory_ =
      attrib_helper.lose_context_when_out_of_memory;

  if (attrib_helper.fail_if_major_perf_caveat &&
      feature_info_->feature_flags().is_swiftshader_for_webgl) {
    group_ = nullptr;
    Destroy(true);
    LOG(ERROR) << "ContextResult::kFatalFailure: "
                  "fail_if_major_perf_caveat + swiftshader";
    return gpu::ContextResult::kFatalFailure;
  }

  if (attrib_helper.context_type == CONTEXT_TYPE_WEBGL2_COMPUTE) {
    group_ = nullptr;
    Destroy(true);
    LOG(ERROR)
        << "ContextResult::kFatalFailure: "
           "webgl2-compute is not supported on validating command decoder.";
    return gpu::ContextResult::kFatalFailure;
  }

  auto result =
      group_->Initialize(this, attrib_helper.context_type, disallowed_features);
  if (result != gpu::ContextResult::kSuccess) {
    group_ = nullptr;
    Destroy(true);
    return result;
  }
  CHECK_GL_ERROR();

  should_use_native_gmb_for_backbuffer_ =
      attrib_helper.should_use_native_gmb_for_backbuffer;
  if (should_use_native_gmb_for_backbuffer_) {
    gpu::ImageFactory* image_factory = group_->image_factory();
    bool supported = false;
    if (image_factory) {
      switch (image_factory->RequiredTextureType()) {
        case GL_TEXTURE_RECTANGLE_ARB:
          supported = feature_info_->feature_flags().arb_texture_rectangle;
          break;
        case GL_TEXTURE_EXTERNAL_OES:
          supported = feature_info_->feature_flags().oes_egl_image_external;
          break;
        case GL_TEXTURE_2D:
          supported = true;
          break;
        default:
          break;
      }
    }

    if (!supported) {
      Destroy(true);
      LOG(ERROR) << "ContextResult::kFatalFailure: "
                    "native gmb format not supported";
      return gpu::ContextResult::kFatalFailure;
    }
  }

  gpu::ImageFactory* image_factory = group_->image_factory();
  if (image_factory && image_factory->SupportsCreateAnonymousImage())
    feature_info_->EnableCHROMIUMTextureStorageImage();

  bool needs_emulation = true;
  transform_feedback_manager_.reset(new TransformFeedbackManager(
      group_->max_transform_feedback_separate_attribs(), needs_emulation));

  if (feature_info_->IsWebGL2OrES3Context()) {
    if (!feature_info_->IsES3Capable()) {
      Destroy(true);
      LOG(ERROR) << "ContextResult::kFatalFailure: "
                    "ES3 is blacklisted/disabled/unsupported by driver.";
      return gpu::ContextResult::kFatalFailure;
    }
    feature_info_->EnableES3Validators();

    frag_depth_explicitly_enabled_ = true;
    draw_buffers_explicitly_enabled_ = true;

    GLuint default_transform_feedback = 0;
    api()->glGenTransformFeedbacksFn(1, &default_transform_feedback);
    state_.default_transform_feedback =
        transform_feedback_manager_->CreateTransformFeedback(
            0, default_transform_feedback);
    api()->glBindTransformFeedbackFn(GL_TRANSFORM_FEEDBACK,
                                     default_transform_feedback);
    state_.bound_transform_feedback = state_.default_transform_feedback.get();
    state_.bound_transform_feedback->SetIsBound(true);
  }
  state_.indexed_uniform_buffer_bindings =
      base::MakeRefCounted<gles2::IndexedBufferBindingHost>(
          group_->max_uniform_buffer_bindings(), GL_UNIFORM_BUFFER,
          needs_emulation);
  state_.indexed_uniform_buffer_bindings->SetIsBound(true);

  state_.InitGenericAttribs(group_->max_vertex_attribs());
  vertex_array_manager_.reset(new VertexArrayManager());

  GLuint default_vertex_attrib_service_id = 0;
  if (features().native_vertex_array_object) {
    api()->glGenVertexArraysOESFn(1, &default_vertex_attrib_service_id);
    api()->glBindVertexArrayOESFn(default_vertex_attrib_service_id);
  }

  state_.default_vertex_attrib_manager =
      CreateVertexAttribManager(0, default_vertex_attrib_service_id, false);

  state_.default_vertex_attrib_manager->Initialize(
      group_->max_vertex_attribs(),
      workarounds().init_vertex_attributes);

  DoBindVertexArrayOES(0);

  framebuffer_manager_.reset(new FramebufferManager(
      group_->max_draw_buffers(), group_->max_color_attachments(),
      group_->framebuffer_completeness_cache()));
  group_->texture_manager()->AddFramebufferManager(framebuffer_manager_.get());

  query_manager_.reset(new GLES2QueryManager(this, feature_info_.get()));

  gpu_fence_manager_.reset(new GpuFenceManager());

  util_.set_num_compressed_texture_formats(
      validators_->compressed_texture_format.GetValues().size());

  if (!gl_version_info().BehavesLikeGLES()) {
    state_.vertex_attrib_manager->SetDriverVertexAttribEnabled(0, true);
  }
  api()->glGenBuffersARBFn(1, &attrib_0_buffer_id_);
  api()->glBindBufferFn(GL_ARRAY_BUFFER, attrib_0_buffer_id_);
  api()->glVertexAttribPointerFn(0, 1, GL_FLOAT, GL_FALSE, 0, nullptr);
  api()->glBindBufferFn(GL_ARRAY_BUFFER, 0);
  api()->glGenBuffersARBFn(1, &fixed_attrib_buffer_id_);

  state_.texture_units.resize(group_->max_texture_units());
  state_.sampler_units.resize(group_->max_texture_units());
  for (uint32_t tt = 0; tt < state_.texture_units.size(); ++tt) {
    api()->glActiveTextureFn(GL_TEXTURE0 + tt);
    TextureRef* ref;
    if (features().oes_egl_image_external ||
        features().nv_egl_stream_consumer_external) {
      ref = texture_manager()->GetDefaultTextureInfo(
          GL_TEXTURE_EXTERNAL_OES);
      state_.texture_units[tt].bound_texture_external_oes = ref;
      api()->glBindTextureFn(GL_TEXTURE_EXTERNAL_OES,
                             ref ? ref->service_id() : 0);
    }
    if (features().arb_texture_rectangle) {
      ref = texture_manager()->GetDefaultTextureInfo(
          GL_TEXTURE_RECTANGLE_ARB);
      state_.texture_units[tt].bound_texture_rectangle_arb = ref;
      api()->glBindTextureFn(GL_TEXTURE_RECTANGLE_ARB,
                             ref ? ref->service_id() : 0);
    }
    ref = texture_manager()->GetDefaultTextureInfo(GL_TEXTURE_CUBE_MAP);
    state_.texture_units[tt].bound_texture_cube_map = ref;
    api()->glBindTextureFn(GL_TEXTURE_CUBE_MAP, ref ? ref->service_id() : 0);
    ref = texture_manager()->GetDefaultTextureInfo(GL_TEXTURE_2D);
    state_.texture_units[tt].bound_texture_2d = ref;
    api()->glBindTextureFn(GL_TEXTURE_2D, ref ? ref->service_id() : 0);
  }
  api()->glActiveTextureFn(GL_TEXTURE0);
  CHECK_GL_ERROR();

  GLint alpha_bits = 0;

  if (offscreen) {
    offscreen_buffer_should_have_alpha_ = attrib_helper.alpha_size > 0;

    bool offscreen_buffer_texture_needs_alpha =
        offscreen_buffer_should_have_alpha_ ||
        (ChromiumImageNeedsRGBEmulation() &&
         attrib_helper.should_use_native_gmb_for_backbuffer);

    if (attrib_helper.samples > 0 && attrib_helper.sample_buffers > 0 &&
        features().chromium_framebuffer_multisample) {
      GLint max_sample_count = 0;
      api()->glGetIntegervFn(GL_MAX_SAMPLES_EXT, &max_sample_count);
      offscreen_target_samples_ = std::min(attrib_helper.samples,
                                           max_sample_count);
    } else {
      offscreen_target_samples_ = 0;
    }
    offscreen_target_buffer_preserved_ = attrib_helper.buffer_preserved;
    offscreen_single_buffer_ = attrib_helper.single_buffer;

    if (gl_version_info().is_es) {
      const bool rgb8_supported = features().oes_rgb8_rgba8;
      if (rgb8_supported && offscreen_target_samples_ > 0) {
        offscreen_target_color_format_ =
            offscreen_buffer_texture_needs_alpha ? GL_RGBA8 : GL_RGB8;
      } else {
        offscreen_target_samples_ = 0;
        offscreen_target_color_format_ =
            offscreen_buffer_texture_needs_alpha ||
                    workarounds().disable_gl_rgb_format
                ? GL_RGBA
                : GL_RGB;
      }

      const bool depth24_stencil8_supported =
          feature_info_->feature_flags().packed_depth24_stencil8;
      VLOG(1) << "GL_OES_packed_depth_stencil "
              << (depth24_stencil8_supported ? "" : "not ") << "supported.";
      if ((attrib_helper.depth_size > 0 || attrib_helper.stencil_size > 0) &&
          depth24_stencil8_supported) {
        offscreen_target_depth_format_ = GL_DEPTH24_STENCIL8;
        offscreen_target_stencil_format_ = 0;
      } else {
        offscreen_target_depth_format_ = attrib_helper.depth_size > 0 ?
            GL_DEPTH_COMPONENT16 : 0;
        offscreen_target_stencil_format_ = attrib_helper.stencil_size > 0 ?
            GL_STENCIL_INDEX8 : 0;
      }
    } else {
      offscreen_target_color_format_ =
          offscreen_buffer_texture_needs_alpha ||
                  workarounds().disable_gl_rgb_format
              ? GL_RGBA
              : GL_RGB;

      const bool depth24_stencil8_supported =
          feature_info_->feature_flags().packed_depth24_stencil8;
      VLOG(1) << "GL_EXT_packed_depth_stencil "
              << (depth24_stencil8_supported ? "" : "not ") << "supported.";

      if ((attrib_helper.depth_size > 0 || attrib_helper.stencil_size > 0) &&
          depth24_stencil8_supported) {
        offscreen_target_depth_format_ = GL_DEPTH24_STENCIL8;
        offscreen_target_stencil_format_ = 0;
      } else {
        offscreen_target_depth_format_ = attrib_helper.depth_size > 0 ?
            GL_DEPTH_COMPONENT : 0;
        offscreen_target_stencil_format_ = attrib_helper.stencil_size > 0 ?
            GL_STENCIL_INDEX : 0;
      }
    }

    offscreen_saved_color_format_ = offscreen_buffer_texture_needs_alpha ||
                                            workarounds().disable_gl_rgb_format
                                        ? GL_RGBA
                                        : GL_RGB;

    max_offscreen_framebuffer_size_ =
        std::min(renderbuffer_manager()->max_renderbuffer_size(),
                 texture_manager()->MaxSizeForTarget(GL_TEXTURE_2D));

    gfx::Size initial_size = attrib_helper.offscreen_framebuffer_size;
    if (initial_size.IsEmpty()) {
      initial_size = gfx::Size(1, 1);
    }

    state_.viewport_width = initial_size.width();
    state_.viewport_height = initial_size.height();
  } else {
    api()->glBindFramebufferEXTFn(GL_FRAMEBUFFER, GetBackbufferServiceId());

    if (!surfaceless_) {
      GLint depth_bits = 0;
      GLint stencil_bits = 0;

      bool default_fb = (GetBackbufferServiceId() == 0);

      if (gl_version_info().is_desktop_core_profile) {
        api()->glGetFramebufferAttachmentParameterivEXTFn(
            GL_FRAMEBUFFER, default_fb ? GL_BACK_LEFT : GL_COLOR_ATTACHMENT0,
            GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE, &alpha_bits);
        api()->glGetFramebufferAttachmentParameterivEXTFn(
            GL_FRAMEBUFFER, default_fb ? GL_DEPTH : GL_DEPTH_ATTACHMENT,
            GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE, &depth_bits);
        api()->glGetFramebufferAttachmentParameterivEXTFn(
            GL_FRAMEBUFFER, default_fb ? GL_STENCIL : GL_STENCIL_ATTACHMENT,
            GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE, &stencil_bits);
      } else {
        api()->glGetIntegervFn(GL_ALPHA_BITS, &alpha_bits);
        api()->glGetIntegervFn(GL_DEPTH_BITS, &depth_bits);
        api()->glGetIntegervFn(GL_STENCIL_BITS, &stencil_bits);
      }

      back_buffer_color_format_ =
          (attrib_helper.alpha_size != 0 && alpha_bits > 0) ? GL_RGBA : GL_RGB;
      back_buffer_has_depth_ = attrib_helper.depth_size != 0 && depth_bits > 0;
      back_buffer_has_stencil_ =
          attrib_helper.stencil_size != 0 && stencil_bits > 0;
      num_stencil_bits_ = stencil_bits;
    } else {
      num_stencil_bits_ = attrib_helper.stencil_size;
    }

    state_.viewport_width = surface->GetSize().width();
    state_.viewport_height = surface->GetSize().height();
  }

  if (!gl_version_info().BehavesLikeGLES()) {
    api()->glEnableFn(GL_VERTEX_PROGRAM_POINT_SIZE);
    api()->glEnableFn(GL_POINT_SPRITE);
  } else if (gl_version_info().is_desktop_core_profile) {
    api()->glEnableFn(GL_PROGRAM_POINT_SIZE);
  }

  if (!workarounds().disable_texture_cube_map_seamless &&
      gl_version_info().IsAtLeastGL(3, 2)) {
    api()->glEnableFn(GL_TEXTURE_CUBE_MAP_SEAMLESS);
  }

  has_robustness_extension_ = features().arb_robustness ||
                              features().khr_robustness ||
                              features().ext_robustness;

  GLint range[2] = {0, 0};
  GLint precision = 0;
  QueryShaderPrecisionFormat(gl_version_info(), GL_FRAGMENT_SHADER,
                             GL_HIGH_FLOAT, range, &precision);
  has_fragment_precision_high_ =
      PrecisionMeetsSpecForHighpFloat(range[0], range[1], precision);

  GLint viewport_params[4] = { 0 };
  api()->glGetIntegervFn(GL_MAX_VIEWPORT_DIMS, viewport_params);
  viewport_max_width_ = viewport_params[0];
  viewport_max_height_ = viewport_params[1];

  api()->glGetFloatvFn(GL_ALIASED_LINE_WIDTH_RANGE, line_width_range_);
  state_.SetLineWidthBounds(line_width_range_[0], line_width_range_[1]);

  if (feature_info_->feature_flags().ext_window_rectangles) {
    GLint max_window_rectangles = -1;
    api()->glGetIntegervFn(GL_MAX_WINDOW_RECTANGLES_EXT,
                           &max_window_rectangles);
    state_.SetMaxWindowRectangles(max_window_rectangles);
  }

  state_.scissor_width = state_.viewport_width;
  state_.scissor_height = state_.viewport_height;

  state_.InitCapabilities(nullptr);
  state_.InitState(nullptr);

  if (offscreen) {
    offscreen_target_frame_buffer_.reset(new BackFramebuffer(this));
    offscreen_target_frame_buffer_->Create();
    if (IsOffscreenBufferMultisampled()) {
      offscreen_target_color_render_buffer_.reset(new BackRenderbuffer(this));
      offscreen_target_color_render_buffer_->Create();
    } else {
      offscreen_target_color_texture_.reset(new BackTexture(this));
      offscreen_target_color_texture_->Create();
    }
    offscreen_target_depth_render_buffer_.reset(new BackRenderbuffer(this));
    offscreen_target_depth_render_buffer_->Create();
    offscreen_target_stencil_render_buffer_.reset(new BackRenderbuffer(this));
    offscreen_target_stencil_render_buffer_->Create();

    if (!offscreen_single_buffer_) {
      offscreen_saved_frame_buffer_.reset(new BackFramebuffer(this));
      offscreen_saved_frame_buffer_->Create();
      offscreen_saved_color_texture_.reset(new BackTexture(this));
      offscreen_saved_color_texture_->Create();
    }

    if (!ResizeOffscreenFramebuffer(
            gfx::Size(state_.viewport_width, state_.viewport_height))) {
      Destroy(true);
      LOG(ERROR) << "ContextResult::kFatalFailure: "
                    "Could not allocate offscreen buffer storage.";
      return gpu::ContextResult::kFatalFailure;
    }
    if (!offscreen_single_buffer_) {
      DCHECK(offscreen_saved_color_format_);
      offscreen_saved_color_texture_->AllocateStorage(
          gfx::Size(1, 1), offscreen_saved_color_format_, true);

      offscreen_saved_frame_buffer_->AttachRenderTexture(
          offscreen_saved_color_texture_.get());
      if (offscreen_saved_frame_buffer_->CheckStatus() !=
          GL_FRAMEBUFFER_COMPLETE) {
        bool was_lost = CheckResetStatus();
        Destroy(true);
        LOG(ERROR) << (was_lost ? "ContextResult::kTransientFailure: "
                                : "ContextResult::kFatalFailure: ")
                   << "Offscreen saved FBO was incomplete.";
        return was_lost ? gpu::ContextResult::kTransientFailure
                        : gpu::ContextResult::kFatalFailure;
      }
    }
  }

  api()->glActiveTextureFn(GL_TEXTURE0 + state_.active_texture_unit);

  DoBindBuffer(GL_ARRAY_BUFFER, 0);
  DoBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  DoBindFramebuffer(GL_FRAMEBUFFER, 0);
  DoBindRenderbuffer(GL_RENDERBUFFER, 0);
  UpdateFramebufferSRGB(nullptr);

  bool call_gl_clear = !surfaceless_ && !offscreen;
#if defined(OS_ANDROID)
  call_gl_clear = surface_->GetHandle();
#endif
  if (call_gl_clear) {
    bool clear_alpha = back_buffer_color_format_ == GL_RGB && alpha_bits > 0;
    if (clear_alpha) {
      api()->glClearColorFn(0.0f, 0.0f, 0.0f, 1.0f);
    }

    api()->glClearFn(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
                     GL_STENCIL_BUFFER_BIT);

    if (clear_alpha) {
      api()->glClearColorFn(0.0f, 0.0f, 0.0f, 0.0f);
    }
  }

  supports_post_sub_buffer_ = surface->SupportsPostSubBuffer();
  if (workarounds()
          .disable_post_sub_buffers_for_onscreen_surfaces &&
      !surface->IsOffscreen())
    supports_post_sub_buffer_ = false;

  supports_swap_buffers_with_bounds_ = surface->SupportsSwapBuffersWithBounds();

  supports_commit_overlay_planes_ = surface->SupportsCommitOverlayPlanes();

  supports_async_swap_ = surface->SupportsAsyncSwap();

  supports_dc_layers_ = !offscreen && surface->SupportsDCLayers();

  if (workarounds().unbind_fbo_on_context_switch) {
    context_->SetUnbindFboOnMakeCurrent();
  }

  if (workarounds().gl_clear_broken) {
    DCHECK(!clear_framebuffer_blit_.get());
    LOCAL_COPY_REAL_GL_ERRORS_TO_WRAPPER("glClearWorkaroundInit");
    clear_framebuffer_blit_ =
        std::make_unique<ClearFramebufferResourceManager>(this);
    if (LOCAL_PEEK_GL_ERROR("glClearWorkaroundInit") != GL_NO_ERROR) {
      LOG(ERROR) << "ContextResult::kFatalFailure: "
                    "glClearWorkaroundInit failed";
      return gpu::ContextResult::kFatalFailure;
    }
  }

  if (group_->gpu_preferences().enable_gpu_driver_debug_logging &&
      feature_info_->feature_flags().khr_debug) {
    InitializeGLDebugLogging(true, GLDebugMessageCallback, &logger_);
  }

  if (feature_info_->feature_flags().chromium_texture_filtering_hint &&
      feature_info_->feature_flags().is_swiftshader) {
    api()->glHintFn(GL_TEXTURE_FILTERING_HINT_CHROMIUM, GL_NICEST);
  }

  return gpu::ContextResult::kSuccess;
}
