void WebContentsImpl::OnUserGesture(RenderWidgetHostImpl* render_widget_host) {
  if (render_widget_host != GetRenderViewHost()->GetWidget())
    return;

  FOR_EACH_OBSERVER(WebContentsObserver, observers_, DidGetUserGesture());

  ResourceDispatcherHostImpl* rdh = ResourceDispatcherHostImpl::Get();
  if (rdh)   
    rdh->OnUserGesture(this);
}
