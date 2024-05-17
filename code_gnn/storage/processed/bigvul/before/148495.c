void WebContentsImpl::OnPepperInstanceCreated(RenderFrameHostImpl* source,
                                              int32_t pp_instance) {
  for (auto& observer : observers_)
    observer.PepperInstanceCreated();
  pepper_playback_observer_->PepperInstanceCreated(source, pp_instance);
}
