void EventReaderLibevdevCros::OnStopped() {
  delegate_->OnLibEvdevCrosStopped(&evdev_, &evstate_);
}
