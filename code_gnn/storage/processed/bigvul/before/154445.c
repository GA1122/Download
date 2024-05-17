GLES2DecoderPassthroughImpl::GLES2DecoderPassthroughImpl(
    DecoderClient* client,
    CommandBufferServiceBase* command_buffer_service,
    Outputter* outputter,
    ContextGroup* group)
    : GLES2Decoder(client, command_buffer_service, outputter),
      commands_to_process_(0),
      debug_marker_manager_(),
      logger_(&debug_marker_manager_,
              base::BindRepeating(&DecoderClient::OnConsoleMessage,
                                  base::Unretained(client),
                                  0),
              group->gpu_preferences().disable_gl_error_limit),
      surface_(),
      context_(),
      offscreen_(false),
      group_(group),
      feature_info_(new FeatureInfo(group->feature_info()->workarounds(),
                                    group->gpu_feature_info())),
      emulated_back_buffer_(nullptr),
      offscreen_single_buffer_(false),
      offscreen_target_buffer_preserved_(false),
      create_color_buffer_count_for_test_(0),
      max_2d_texture_size_(0),
      bound_draw_framebuffer_(0),
      bound_read_framebuffer_(0),
      gpu_decoder_category_(TRACE_EVENT_API_GET_CATEGORY_GROUP_ENABLED(
          TRACE_DISABLED_BY_DEFAULT("gpu.decoder"))),
      gpu_trace_level_(2),
      gpu_trace_commands_(false),
      gpu_debug_commands_(false),
      has_robustness_extension_(false),
      context_lost_(false),
      reset_by_robustness_extension_(false),
      lose_context_when_out_of_memory_(false),
      weak_ptr_factory_(this) {
  DCHECK(client);
  DCHECK(group);
}
