void WebContentsImpl::RenderFrameDeleted(RenderFrameHost* render_frame_host) {
  is_notifying_observers_ = true;
  for (auto& observer : observers_)
    observer.RenderFrameDeleted(render_frame_host);
  is_notifying_observers_ = false;
#if BUILDFLAG(ENABLE_PLUGINS)
  pepper_playback_observer_->RenderFrameDeleted(render_frame_host);
#endif
}
