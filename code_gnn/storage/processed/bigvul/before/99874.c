WebPluginDelegatePepper::WebPluginDelegatePepper(
    const base::WeakPtr<RenderView>& render_view,
    NPAPI::PluginInstance *instance)
    : render_view_(render_view),
      plugin_(NULL),
      instance_(instance),
      nested_delegate_(NULL),
      last_command_buffer_put_offset_(-1) {
  memset(&window_, 0, sizeof(window_));
  instance->set_windowless(true);
  instance->set_transparent(true);
}
