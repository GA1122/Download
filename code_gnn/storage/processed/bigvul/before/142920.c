void HTMLMediaElement::StopPeriodicTimers() {
  progress_event_timer_.Stop();
  playback_progress_timer_.Stop();
  if (lazy_load_visibility_observer_) {
    lazy_load_visibility_observer_->Stop();
    lazy_load_visibility_observer_ = nullptr;
  }
}
