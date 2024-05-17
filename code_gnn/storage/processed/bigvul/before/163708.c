void WebContentsImpl::NotifyFrameSwapped(RenderFrameHost* old_host,
                                         RenderFrameHost* new_host) {
#if defined(OS_ANDROID)
  RenderFrameHostImpl* importance_host = static_cast<RenderFrameHostImpl*>(
      old_host ? old_host : new_host->GetParent());
  if (importance_host) {
    static_cast<RenderFrameHostImpl*>(new_host)
        ->GetRenderWidgetHost()
        ->SetImportance(importance_host->GetRenderWidgetHost()->importance());
  }
#endif
  for (auto& observer : observers_)
    observer.RenderFrameHostChanged(old_host, new_host);
}
