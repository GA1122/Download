void DeviceTokenFetcher::DoWork() {
  switch (state_) {
    case STATE_INACTIVE:
    case STATE_TOKEN_AVAILABLE:
      break;
    case STATE_UNMANAGED:
    case STATE_ERROR:
    case STATE_TEMPORARY_ERROR:
    case STATE_BAD_AUTH:
      FetchTokenInternal();
      break;
  }
}
