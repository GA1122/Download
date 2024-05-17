void WebContentsImpl::ExitFullscreenMode(bool will_cause_resize) {
  RenderWidgetHostView* const widget_view = GetFullscreenRenderWidgetHostView();
  if (widget_view) {
    RenderWidgetHostImpl::From(widget_view->GetRenderWidgetHost())
        ->ShutdownAndDestroyWidget(true);
  }

  if (delegate_) {
    delegate_->ExitFullscreenModeForTab(this);

    if (keyboard_lock_widget_)
      delegate_->CancelKeyboardLockRequest(this);
  }

  if (!will_cause_resize) {
    if (RenderWidgetHostView* rwhv = GetRenderWidgetHostView()) {
        if (RenderWidgetHost* render_widget_host = rwhv->GetRenderWidgetHost())
          render_widget_host->SynchronizeVisualProperties();
    }
  }

  current_fullscreen_frame_tree_node_id_ = RenderFrameHost::kNoFrameTreeNodeId;

  for (auto& observer : observers_) {
    observer.DidToggleFullscreenModeForTab(IsFullscreenForCurrentTab(),
                                           will_cause_resize);
  }

  if (display_cutout_host_impl_)
    display_cutout_host_impl_->DidExitFullscreen();
}
