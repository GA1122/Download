WebPluginDelegateProxy::WebPluginDelegateProxy(
    const std::string& mime_type,
    const base::WeakPtr<RenderView>& render_view)
    : render_view_(render_view),
      plugin_(NULL),
      uses_shared_bitmaps_(false),
      window_(gfx::kNullPluginWindow),
      mime_type_(mime_type),
      instance_id_(MSG_ROUTING_NONE),
      npobject_(NULL),
      sad_plugin_(NULL),
      invalidate_pending_(false),
      transparent_(false),
      page_url_(render_view_->webview()->mainFrame()->url()) {
}
