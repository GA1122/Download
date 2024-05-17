void WebContentsImpl::ShowCreatedWidget(int route_id,
                                        bool is_fullscreen,
                                        const gfx::Rect& initial_rect) {
  RenderWidgetHostViewBase* widget_host_view =
      static_cast<RenderWidgetHostViewBase*>(GetCreatedWidget(route_id));
  if (!widget_host_view)
    return;

  RenderWidgetHostView* view = NULL;
  BrowserPluginGuest* guest = GetBrowserPluginGuest();
  if (guest && guest->embedder_web_contents()) {
    view = guest->embedder_web_contents()->GetRenderWidgetHostView();
  } else {
    view = GetRenderWidgetHostView();
  }

  if (is_fullscreen) {
    DCHECK_EQ(MSG_ROUTING_NONE, fullscreen_widget_routing_id_);
    view_->StoreFocus();
    fullscreen_widget_routing_id_ = route_id;
    if (delegate_ && delegate_->EmbedsFullscreenWidget()) {
      widget_host_view->InitAsChild(GetRenderWidgetHostView()->GetNativeView());
      delegate_->EnterFullscreenModeForTab(this, GURL());
    } else {
      widget_host_view->InitAsFullscreen(view);
    }
    FOR_EACH_OBSERVER(WebContentsObserver,
                      observers_,
                      DidShowFullscreenWidget(route_id));
    if (!widget_host_view->HasFocus())
      widget_host_view->Focus();
  } else {
    widget_host_view->InitAsPopup(view, initial_rect);
  }

  RenderWidgetHostImpl* render_widget_host_impl =
      RenderWidgetHostImpl::From(widget_host_view->GetRenderWidgetHost());
  render_widget_host_impl->Init();
  render_widget_host_impl->set_allow_privileged_mouse_lock(is_fullscreen);

#if defined(OS_MACOSX)
  base::mac::NSObjectRelease(widget_host_view->GetNativeView());
#endif
}
