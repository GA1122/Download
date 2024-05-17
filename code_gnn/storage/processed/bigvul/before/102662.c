void DeviceTokenFetcher::FetchToken() {
  SetState(STATE_INACTIVE);
  FetchTokenInternal();
}
