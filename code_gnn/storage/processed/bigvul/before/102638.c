void CloudPolicyController::OnError(DeviceManagementBackend::ErrorCode code) {
  switch (code) {
    case DeviceManagementBackend::kErrorServiceDeviceNotFound:
    case DeviceManagementBackend::kErrorServiceManagementTokenInvalid: {
      LOG(WARNING) << "The device token was either invalid or unknown to the "
                   << "device manager, re-registering device.";
      SetState(STATE_TOKEN_ERROR);
      break;
    }
    case DeviceManagementBackend::kErrorServiceManagementNotSupported: {
      VLOG(1) << "The device is no longer managed.";
      token_fetcher_->SetUnmanagedState();
      SetState(STATE_TOKEN_UNMANAGED);
      break;
    }
    case DeviceManagementBackend::kErrorServicePolicyNotFound:
    case DeviceManagementBackend::kErrorRequestInvalid:
    case DeviceManagementBackend::kErrorServiceActivationPending:
    case DeviceManagementBackend::kErrorResponseDecoding:
    case DeviceManagementBackend::kErrorHttpStatus: {
      VLOG(1) << "An error in the communication with the policy server occurred"
              << ", will retry in a few hours.";
      SetState(STATE_POLICY_UNAVAILABLE);
      break;
    }
    case DeviceManagementBackend::kErrorRequestFailed:
    case DeviceManagementBackend::kErrorTemporaryUnavailable: {
      VLOG(1) << "A temporary error in the communication with the policy server"
              << " occurred.";
      SetState(STATE_POLICY_ERROR);
    }
  }
}
