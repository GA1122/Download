void WebContentsImpl::EnterFullscreenMode(const GURL& origin) {
  RenderWidgetHostView* const widget_view = GetFullscreenRenderWidgetHostView();
  if (widget_view) {
    RenderWidgetHostImpl::From(widget_view->GetRenderWidgetHost())
        ->ShutdownAndDestroyWidget(true);
  }

  if (delegate_)
    delegate_->EnterFullscreenModeForTab(this, origin);

  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    DidToggleFullscreenModeForTab(
                        IsFullscreenForCurrentTab(
                            GetRenderViewHost()->GetWidget()),
                        false));
}
