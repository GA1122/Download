WebPluginDelegatePepper::WebPluginDelegatePepper(
    const base::WeakPtr<RenderView>& render_view,
    NPAPI::PluginInstance *instance)
    : render_view_(render_view),
      plugin_(NULL),
      instance_(instance),
      nested_delegate_(NULL),
#if defined(ENABLE_GPU)
      command_buffer_(NULL),
#endif
      find_identifier_(-1),
      method_factory3d_(ALLOW_THIS_IN_INITIALIZER_LIST(this)),
      current_choose_file_callback_(NULL),
      current_choose_file_user_data_(NULL) {
  memset(&window_, 0, sizeof(window_));
  instance->set_windowless(true);
  instance->set_transparent(true);
}
