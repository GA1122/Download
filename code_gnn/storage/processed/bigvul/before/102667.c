void DeviceTokenFetcher::SetUnmanagedState() {
  cache_->SetUnmanaged();
   SetState(STATE_UNMANAGED);
 }
