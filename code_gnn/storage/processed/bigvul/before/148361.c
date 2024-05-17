void WebContentsImpl::DocumentAvailableInMainFrame(
    RenderViewHost* render_view_host) {
  for (auto& observer : observers_)
    observer.DocumentAvailableInMainFrame();
}
