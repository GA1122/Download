void WebContentsImpl::ShowCreatedWidget(int process_id,
                                        int route_id,
                                        bool is_fullscreen,
                                        const gfx::Rect& initial_rect) {
  RenderWidgetHostViewBase* widget_host_view =
      static_cast<RenderWidgetHostViewBase*>(
          GetCreatedWidget(process_id, route_id));
  if (!widget_host_view)
    return;

  RenderWidgetHostView* view = nullptr;
  if (GetOuterWebContents()) {
    view = GetOuterWebContents()->GetRenderWidgetHostView();
  } else {
    view = GetRenderWidgetHostView();
  }

  if (is_fullscreen) {
    DCHECK_EQ(MSG_ROUTING_NONE, fullscreen_widget_routing_id_);
    view_->StoreFocus();
    fullscreen_widget_process_id_ =
        widget_host_view->GetRenderWidgetHost()->GetProcess()->GetID();
    fullscreen_widget_routing_id_ = route_id;
    if (delegate_ && delegate_->EmbedsFullscreenWidget()) {
      widget_host_view->InitAsChild(GetRenderWidgetHostView()->GetNativeView());
      delegate_->EnterFullscreenModeForTab(this, GURL());
    } else {
      widget_host_view->InitAsFullscreen(view);
    }
    for (auto& observer : observers_)
      observer.DidShowFullscreenWidget();
    if (!widget_host_view->HasFocus())
      widget_host_view->Focus();
  } else {
    widget_host_view->InitAsPopup(view, initial_rect);
  }

  RenderWidgetHostImpl* render_widget_host_impl = widget_host_view->host();
  render_widget_host_impl->Init();
  render_widget_host_impl->set_allow_privileged_mouse_lock(is_fullscreen);

#if defined(OS_MACOSX)
  base::mac::NSObjectRelease(widget_host_view->GetNativeView());
#endif
}
