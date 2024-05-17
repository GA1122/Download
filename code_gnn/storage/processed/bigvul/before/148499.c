void WebContentsImpl::OnPepperStopsPlayback(RenderFrameHostImpl* source,
                                            int32_t pp_instance) {
  pepper_playback_observer_->PepperStopsPlayback(source, pp_instance);
}
