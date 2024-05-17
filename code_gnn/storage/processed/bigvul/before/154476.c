gpu::ContextResult GLES2DecoderPassthroughImpl::Initialize(
    const scoped_refptr<gl::GLSurface>& surface,
    const scoped_refptr<gl::GLContext>& context,
    bool offscreen,
    const DisallowedFeatures& disallowed_features,
    const ContextCreationAttribs& attrib_helper) {
  TRACE_EVENT0("gpu", "GLES2DecoderPassthroughImpl::Initialize");
  DCHECK(context->IsCurrent(surface.get()));
  api_ = gl::g_current_gl_context;
  context_ = context;
  surface_ = surface;
  offscreen_ = offscreen;

  bool log_non_errors =
      group_->gpu_preferences().enable_gpu_driver_debug_logging;
  InitializeGLDebugLogging(log_non_errors, PassthroughGLDebugMessageCallback,
                           this);

  gpu_tracer_.reset(new GPUTracer(this));

  gpu_fence_manager_.reset(new GpuFenceManager());

  multi_draw_manager_.reset(
      new MultiDrawManager(MultiDrawManager::IndexStorageType::Pointer));

  auto result =
      group_->Initialize(this, attrib_helper.context_type, disallowed_features);
  if (result != gpu::ContextResult::kSuccess) {
    group_ = nullptr;
    Destroy(true);
    return result;
  }

  if (IsWebGLContextType(attrib_helper.context_type)) {
    gfx::ExtensionSet requestable_extensions(
        gl::GetRequestableGLExtensionsFromCurrentContext());

    static constexpr const char* kRequiredFunctionalityExtensions[] = {
        "GL_ANGLE_memory_size",   "GL_CHROMIUM_bind_uniform_location",
        "GL_CHROMIUM_sync_query", "GL_EXT_debug_marker",
        "GL_KHR_debug",           "GL_NV_fence",
    };
    RequestExtensions(api(), requestable_extensions,
                      kRequiredFunctionalityExtensions,
                      base::size(kRequiredFunctionalityExtensions));

    if (request_optional_extensions_) {
      static constexpr const char* kOptionalFunctionalityExtensions[] = {
          "GL_ANGLE_depth_texture",
          "GL_ANGLE_framebuffer_blit",
          "GL_ANGLE_framebuffer_multisample",
          "GL_ANGLE_instanced_arrays",
          "GL_ANGLE_pack_reverse_row_order",
          "GL_ANGLE_texture_compression_dxt1",
          "GL_ANGLE_texture_compression_dxt3",
          "GL_ANGLE_texture_compression_dxt5",
          "GL_ANGLE_texture_usage",
          "GL_ANGLE_translated_shader_source",
          "GL_CHROMIUM_framebuffer_mixed_samples",
          "GL_CHROMIUM_path_rendering",
          "GL_EXT_blend_minmax",
          "GL_EXT_discard_framebuffer",
          "GL_EXT_disjoint_timer_query",
          "GL_EXT_occlusion_query_boolean",
          "GL_EXT_sRGB",
          "GL_EXT_sRGB_write_control",
          "GL_EXT_texture_compression_dxt1",
          "GL_EXT_texture_compression_s3tc_srgb",
          "GL_EXT_texture_format_BGRA8888",
          "GL_EXT_texture_norm16",
          "GL_EXT_texture_rg",
          "GL_EXT_texture_sRGB_decode",
          "GL_EXT_texture_storage",
          "GL_EXT_unpack_subimage",
          "GL_KHR_parallel_shader_compile",
          "GL_KHR_robust_buffer_access_behavior",
          "GL_KHR_texture_compression_astc_hdr",
          "GL_KHR_texture_compression_astc_ldr",
          "GL_NV_pack_subimage",
          "GL_OES_compressed_ETC1_RGB8_texture",
          "GL_OES_depth32",
          "GL_OES_EGL_image",
          "GL_OES_EGL_image_external",
          "GL_OES_EGL_image_external_essl3",
          "GL_OES_packed_depth_stencil",
          "GL_OES_rgb8_rgba8",
          "GL_OES_vertex_array_object",
          "NV_EGL_stream_consumer_external",
      };
      RequestExtensions(api(), requestable_extensions,
                        kOptionalFunctionalityExtensions,
                        base::size(kOptionalFunctionalityExtensions));
    }

    context->ReinitializeDynamicBindings();
  }

  feature_info_->Initialize(attrib_helper.context_type,
                            true  ,
                            DisallowedFeatures());


#define FAIL_INIT_IF_NOT(feature, message)                       \
  if (!(feature)) {                                              \
    Destroy(true);                                               \
    LOG(ERROR) << "ContextResult::kFatalFailure: " << (message); \
    return gpu::ContextResult::kFatalFailure;                    \
  }

  FAIL_INIT_IF_NOT(feature_info_->feature_flags().angle_robust_client_memory,
                   "missing GL_ANGLE_robust_client_memory");
  FAIL_INIT_IF_NOT(
      feature_info_->feature_flags().chromium_bind_generates_resource,
      "missing GL_CHROMIUM_bind_generates_resource");
  FAIL_INIT_IF_NOT(feature_info_->feature_flags().chromium_copy_texture,
                   "missing GL_CHROMIUM_copy_texture");
  FAIL_INIT_IF_NOT(feature_info_->feature_flags().angle_client_arrays,
                   "missing GL_ANGLE_client_arrays");
  FAIL_INIT_IF_NOT(api()->glIsEnabledFn(GL_CLIENT_ARRAYS_ANGLE) == GL_FALSE,
                   "GL_ANGLE_client_arrays shouldn't be enabled");
  FAIL_INIT_IF_NOT(feature_info_->feature_flags().angle_webgl_compatibility ==
                       IsWebGLContextType(attrib_helper.context_type),
                   "missing GL_ANGLE_webgl_compatibility");
  FAIL_INIT_IF_NOT(feature_info_->feature_flags().angle_request_extension,
                   "missing  GL_ANGLE_request_extension");
  FAIL_INIT_IF_NOT(feature_info_->feature_flags().khr_debug,
                   "missing GL_KHR_debug");
  FAIL_INIT_IF_NOT(
      !IsWebGL2ComputeContextType(attrib_helper.context_type) ||
          feature_info_->feature_flags().khr_robust_buffer_access_behavior,
      "missing GL_KHR_robust_buffer_access_behavior");
  FAIL_INIT_IF_NOT(!attrib_helper.enable_oop_rasterization,
                   "oop rasterization not supported");

#undef FAIL_INIT_IF_NOT

  bind_generates_resource_ = group_->bind_generates_resource();

  resources_ = group_->passthrough_resources();

  mailbox_manager_ = group_->mailbox_manager();

  GLint num_texture_units = 0;
  api()->glGetIntegervFn(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
                         &num_texture_units);
  if (num_texture_units > static_cast<GLint>(kMaxTextureUnits)) {
    Destroy(true);
    LOG(ERROR) << "kMaxTextureUnits (" << kMaxTextureUnits
               << ") must be at least GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS ("
               << num_texture_units << ").";
    return gpu::ContextResult::kFatalFailure;
  }

  active_texture_unit_ = 0;

  bound_buffers_[GL_ARRAY_BUFFER] = 0;
  bound_buffers_[GL_ELEMENT_ARRAY_BUFFER] = 0;
  if (feature_info_->gl_version_info().IsAtLeastGLES(3, 0) ||
      feature_info_->feature_flags().ext_pixel_buffer_object) {
    bound_buffers_[GL_PIXEL_PACK_BUFFER] = 0;
    bound_buffers_[GL_PIXEL_UNPACK_BUFFER] = 0;
  }
  if (feature_info_->gl_version_info().IsAtLeastGLES(3, 0)) {
    bound_buffers_[GL_COPY_READ_BUFFER] = 0;
    bound_buffers_[GL_COPY_WRITE_BUFFER] = 0;
    bound_buffers_[GL_TRANSFORM_FEEDBACK_BUFFER] = 0;
    bound_buffers_[GL_UNIFORM_BUFFER] = 0;
  }
  if (feature_info_->gl_version_info().IsAtLeastGLES(3, 1)) {
    bound_buffers_[GL_ATOMIC_COUNTER_BUFFER] = 0;
    bound_buffers_[GL_SHADER_STORAGE_BUFFER] = 0;
    bound_buffers_[GL_DRAW_INDIRECT_BUFFER] = 0;
    bound_buffers_[GL_DISPATCH_INDIRECT_BUFFER] = 0;
  }

  if (feature_info_->feature_flags().chromium_texture_filtering_hint &&
      feature_info_->feature_flags().is_swiftshader) {
    api()->glHintFn(GL_TEXTURE_FILTERING_HINT_CHROMIUM, GL_NICEST);
  }

  has_robustness_extension_ = feature_info_->feature_flags().khr_robustness ||
                              feature_info_->feature_flags().ext_robustness;
  lose_context_when_out_of_memory_ =
      attrib_helper.lose_context_when_out_of_memory;

  api()->glGetIntegervFn(GL_MAX_TEXTURE_SIZE, &max_2d_texture_size_);
  api()->glGetIntegervFn(GL_MAX_RENDERBUFFER_SIZE, &max_renderbuffer_size_);
  max_offscreen_framebuffer_size_ =
      std::min(max_2d_texture_size_, max_renderbuffer_size_);

  if (offscreen_) {
    offscreen_single_buffer_ = attrib_helper.single_buffer;
    offscreen_target_buffer_preserved_ = attrib_helper.buffer_preserved;
    const bool multisampled_framebuffers_supported =
        feature_info_->feature_flags().chromium_framebuffer_multisample;
    if (attrib_helper.samples > 0 && attrib_helper.sample_buffers > 0 &&
        multisampled_framebuffers_supported && !offscreen_single_buffer_) {
      GLint max_sample_count = 0;
      api()->glGetIntegervFn(GL_MAX_SAMPLES_EXT, &max_sample_count);
      emulated_default_framebuffer_format_.samples =
          std::min(attrib_helper.samples, max_sample_count);
    }

    const bool rgb8_supported = feature_info_->feature_flags().oes_rgb8_rgba8;
    const bool alpha_channel_requested = attrib_helper.alpha_size > 0;
    if (rgb8_supported && emulated_default_framebuffer_format_.samples > 0) {
      emulated_default_framebuffer_format_.color_renderbuffer_internal_format =
          alpha_channel_requested ? GL_RGBA8 : GL_RGB8;
    } else {
      emulated_default_framebuffer_format_.samples = 0;
    }

    emulated_default_framebuffer_format_.color_texture_internal_format =
        alpha_channel_requested ? GL_RGBA : GL_RGB;
    emulated_default_framebuffer_format_.color_texture_format =
        emulated_default_framebuffer_format_.color_texture_internal_format;
    emulated_default_framebuffer_format_.color_texture_type = GL_UNSIGNED_BYTE;

    const bool depth24_stencil8_supported =
        feature_info_->feature_flags().packed_depth24_stencil8;
    if ((attrib_helper.depth_size > 0 || attrib_helper.stencil_size > 0) &&
        depth24_stencil8_supported) {
      emulated_default_framebuffer_format_.depth_stencil_internal_format =
          GL_DEPTH24_STENCIL8;
    } else {
      if (attrib_helper.depth_size > 0) {
        emulated_default_framebuffer_format_.depth_internal_format =
            GL_DEPTH_COMPONENT16;
      }
      if (attrib_helper.stencil_size > 0) {
        emulated_default_framebuffer_format_.stencil_internal_format =
            GL_STENCIL_INDEX8;
      }
    }

    CheckErrorCallbackState();
    emulated_back_buffer_ = std::make_unique<EmulatedDefaultFramebuffer>(
        api(), emulated_default_framebuffer_format_, feature_info_.get(),
        supports_separate_fbo_bindings_);
    gfx::Size initial_size(
        std::max(1, attrib_helper.offscreen_framebuffer_size.width()),
        std::max(1, attrib_helper.offscreen_framebuffer_size.height()));
    if (!emulated_back_buffer_->Resize(initial_size, feature_info_.get())) {
      bool was_lost = CheckResetStatus();
      Destroy(true);
      LOG(ERROR) << (was_lost ? "ContextResult::kTransientFailure: "
                              : "ContextResult::kFatalFailure: ")
                 << "Resize of emulated back buffer failed";
      return was_lost ? gpu::ContextResult::kTransientFailure
                      : gpu::ContextResult::kFatalFailure;
    }

    if (CheckErrorCallbackState()) {
      Destroy(true);
      LOG(ERROR)
          << "ContextResult::kFatalFailure: "
             "Creation of the offscreen framebuffer failed because errors were "
             "generated.";
      return gpu::ContextResult::kFatalFailure;
    }

    framebuffer_id_map_.SetIDMapping(
        0, emulated_back_buffer_->framebuffer_service_id);

    api()->glBindFramebufferEXTFn(
        GL_FRAMEBUFFER, emulated_back_buffer_->framebuffer_service_id);
    api()->glViewportFn(0, 0, attrib_helper.offscreen_framebuffer_size.width(),
                        attrib_helper.offscreen_framebuffer_size.height());
  }

  api()->glGetIntegervFn(GL_VIEWPORT, viewport_);
  api()->glGetIntegervFn(GL_SCISSOR_BOX, scissor_);
  ApplySurfaceDrawOffset();

  set_initialized();
  return gpu::ContextResult::kSuccess;
}
