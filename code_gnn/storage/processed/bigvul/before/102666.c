void DeviceTokenFetcher::OnError(DeviceManagementBackend::ErrorCode code) {
  switch (code) {
    case DeviceManagementBackend::kErrorServiceManagementNotSupported:
      cache_->SetUnmanaged();
      SetState(STATE_UNMANAGED);
      break;
    case DeviceManagementBackend::kErrorRequestFailed:
    case DeviceManagementBackend::kErrorTemporaryUnavailable:
    case DeviceManagementBackend::kErrorServiceDeviceNotFound:
      SetState(STATE_TEMPORARY_ERROR);
      break;
    case DeviceManagementBackend::kErrorServiceManagementTokenInvalid:
      SetState(STATE_BAD_AUTH);
      break;
    default:
      SetState(STATE_ERROR);
  }
}
