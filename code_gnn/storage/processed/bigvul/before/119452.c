void ScreenOrientationDispatcherHost::OnOrientationChange() {
  if (provider_)
    provider_->OnOrientationChange();
}
