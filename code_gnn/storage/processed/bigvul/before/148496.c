void WebContentsImpl::OnPepperInstanceDeleted(RenderFrameHostImpl* source,
                                              int32_t pp_instance) {
  for (auto& observer : observers_)
    observer.PepperInstanceDeleted();
  pepper_playback_observer_->PepperInstanceDeleted(source, pp_instance);
}
