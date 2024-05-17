void WebContentsImpl::FullscreenStateChanged(RenderFrameHost* rfh,
                                             bool is_fullscreen) {
  RenderFrameHostImpl* frame = static_cast<RenderFrameHostImpl*>(rfh);

  if (is_fullscreen) {
    if (!base::ContainsKey(fullscreen_frames_, frame)) {
      fullscreen_frames_.insert(frame);
      FullscreenFrameSetUpdated();
    }
    return;
  }

  bool changed = false;
  base::EraseIf(fullscreen_frames_, [&](RenderFrameHostImpl* rfh) {
    for (auto* current = rfh; current; current = current->GetParent()) {
      if (current == frame) {
        changed = true;
        return true;
      }
    }
    return false;
  });

  if (changed)
    FullscreenFrameSetUpdated();
}
