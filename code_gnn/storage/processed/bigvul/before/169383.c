void WaitForResizeComplete(WebContents* web_contents) {
  RenderWidgetHostImpl* widget_host = RenderWidgetHostImpl::From(
      web_contents->GetRenderViewHost()->GetWidget());
  if (!IsResizeComplete(widget_host)) {
    WindowedNotificationObserver resize_observer(
        NOTIFICATION_RENDER_WIDGET_HOST_DID_COMPLETE_RESIZE_OR_REPAINT,
        base::Bind(IsResizeComplete, widget_host));
    resize_observer.Wait();
  }
}
