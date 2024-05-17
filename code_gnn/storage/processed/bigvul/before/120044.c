bool FileBrowserPrivateGetDriveConnectionStateFunction::RunSync() {
  api::file_browser_private::DriveConnectionState result;

  switch (drive::util::GetDriveConnectionStatus(GetProfile())) {
    case drive::util::DRIVE_DISCONNECTED_NOSERVICE:
      result.type = kDriveConnectionTypeOffline;
      result.reason.reset(new std::string(kDriveConnectionReasonNoService));
      break;
    case drive::util::DRIVE_DISCONNECTED_NONETWORK:
      result.type = kDriveConnectionTypeOffline;
      result.reason.reset(new std::string(kDriveConnectionReasonNoNetwork));
      break;
    case drive::util::DRIVE_DISCONNECTED_NOTREADY:
      result.type = kDriveConnectionTypeOffline;
      result.reason.reset(new std::string(kDriveConnectionReasonNotReady));
      break;
    case drive::util::DRIVE_CONNECTED_METERED:
      result.type = kDriveConnectionTypeMetered;
      break;
    case drive::util::DRIVE_CONNECTED:
      result.type = kDriveConnectionTypeOnline;
      break;
  }

  results_ = api::file_browser_private::GetDriveConnectionState::Results::
      Create(result);

  drive::EventLogger* logger = file_manager::util::GetLogger(GetProfile());
  if (logger)
    logger->Log(logging::LOG_INFO, "%s succeeded.", name().c_str());
  return true;
}
