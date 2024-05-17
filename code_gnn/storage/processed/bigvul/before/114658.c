WebPluginDelegateProxy::WebPluginDelegateProxy(
    const std::string& mime_type,
    const base::WeakPtr<RenderViewImpl>& render_view)
    : render_view_(render_view),
      plugin_(NULL),
      uses_shared_bitmaps_(false),
#if defined(OS_MACOSX)
      uses_compositor_(false),
#endif
      window_(gfx::kNullPluginWindow),
      mime_type_(mime_type),
      instance_id_(MSG_ROUTING_NONE),
      npobject_(NULL),
      sad_plugin_(NULL),
      invalidate_pending_(false),
      transparent_(false),
      front_buffer_index_(0),
      page_url_(render_view_->webview()->mainFrame()->document().url()) {
}
