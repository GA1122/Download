void WebContentsImpl::OnPepperStartsPlayback(RenderFrameHostImpl* source,
                                             int32_t pp_instance) {
  pepper_playback_observer_->PepperStartsPlayback(source, pp_instance);
}
