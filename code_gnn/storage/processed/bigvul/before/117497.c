const char* UpdateStatusToString(
    UpdateEngineClient::UpdateStatusOperation status) {
  switch (status) {
    case UpdateEngineClient::UPDATE_STATUS_IDLE:
      return "idle";
    case UpdateEngineClient::UPDATE_STATUS_CHECKING_FOR_UPDATE:
      return "checking for update";
    case UpdateEngineClient::UPDATE_STATUS_UPDATE_AVAILABLE:
      return "update available";
    case UpdateEngineClient::UPDATE_STATUS_DOWNLOADING:
      return "downloading";
    case UpdateEngineClient::UPDATE_STATUS_VERIFYING:
      return "verifying";
    case UpdateEngineClient::UPDATE_STATUS_FINALIZING:
      return "finalizing";
    case UpdateEngineClient::UPDATE_STATUS_UPDATED_NEED_REBOOT:
      return "updated need reboot";
    case UpdateEngineClient::UPDATE_STATUS_REPORTING_ERROR_EVENT:
      return "reporting error event";
    default:
      return "unknown";
  }
}
