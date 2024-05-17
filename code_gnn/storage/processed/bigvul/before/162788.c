ContextState::ContextState(FeatureInfo* feature_info,
                           ErrorStateClient* error_state_client,
                           Logger* logger)
    : active_texture_unit(0),
      bound_renderbuffer_valid(false),
      pack_reverse_row_order(false),
      ignore_cached_state(false),
      fbo_binding_for_scissor_workaround_dirty(false),
      feature_info_(feature_info),
      error_state_(ErrorState::Create(error_state_client, logger)) {
  Initialize();
}
