ExtensionHelper::ExtensionHelper(content::RenderView* render_view,
                                 ExtensionDispatcher* extension_dispatcher)
    : content::RenderViewObserver(render_view),
      content::RenderViewObserverTracker<ExtensionHelper>(render_view),
      extension_dispatcher_(extension_dispatcher),
      pending_app_icon_requests_(0),
      view_type_(content::VIEW_TYPE_INVALID),
      browser_window_id_(-1) {
}
