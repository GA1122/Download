void WebContentsImpl::EnterFullscreenMode(const GURL& origin) {
  RenderWidgetHostView* const widget_view = GetFullscreenRenderWidgetHostView();
  if (widget_view) {
    RenderWidgetHostImpl::From(widget_view->GetRenderWidgetHost())
        ->ShutdownAndDestroyWidget(true);
  }

  if (delegate_)
    delegate_->EnterFullscreenModeForTab(this, origin);

  for (auto& observer : observers_)
    observer.DidToggleFullscreenModeForTab(IsFullscreenForCurrentTab(), false);
}
