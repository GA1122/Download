void WebContentsImpl::OnUpdateFaviconURL(
    const std::vector<FaviconURL>& candidates) {
  RenderViewHostImpl* rvhi =
      static_cast<RenderViewHostImpl*>(render_view_message_source_);
  if (!rvhi->is_active())
    return;

  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    DidUpdateFaviconURL(candidates));
}
