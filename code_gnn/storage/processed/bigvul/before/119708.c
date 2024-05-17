void ResetScreenHandler::HandleOnPowerwash(bool rollback_checked) {
  if (rollback_available_ && (rollback_checked || reboot_was_requested_)) {
      CallJS("updateViewOnRollbackCall");
      DBusThreadManager::Get()->GetUpdateEngineClient()->AddObserver(this);
      chromeos::DBusThreadManager::Get()->GetUpdateEngineClient()->Rollback();
  } else {
    if (rollback_checked && !rollback_available_) {
      NOTREACHED() <<
          "Rollback was checked but not available. Starting powerwash.";
    }
    chromeos::DBusThreadManager::Get()->GetSessionManagerClient()->
        StartDeviceWipe();
  }
}
