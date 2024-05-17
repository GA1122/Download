void WebContentsImpl::ExitFullscreenMode(bool will_cause_resize) {
  RenderWidgetHostView* const widget_view = GetFullscreenRenderWidgetHostView();
  if (widget_view) {
    RenderWidgetHostImpl::From(widget_view->GetRenderWidgetHost())
        ->ShutdownAndDestroyWidget(true);
  }

#if defined(OS_ANDROID)
  ContentVideoView* video_view = ContentVideoView::GetInstance();
  if (video_view != NULL)
    video_view->ExitFullscreen();
#endif

  if (delegate_)
    delegate_->ExitFullscreenModeForTab(this);

  if (!will_cause_resize) {
    if (RenderWidgetHostView* rwhv = GetRenderWidgetHostView()) {
        if (RenderWidgetHost* render_widget_host = rwhv->GetRenderWidgetHost())
          render_widget_host->WasResized();
    }
  }

  for (auto& observer : observers_) {
    observer.DidToggleFullscreenModeForTab(IsFullscreenForCurrentTab(),
                                           will_cause_resize);
  }
}
