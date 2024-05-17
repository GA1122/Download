NavigationEntryImpl::RestoreType ControllerRestoreTypeToEntryType(
    NavigationController::RestoreType type) {
  switch (type) {
    case NavigationController::RESTORE_CURRENT_SESSION:
      return NavigationEntryImpl::RESTORE_CURRENT_SESSION;
    case NavigationController::RESTORE_LAST_SESSION_EXITED_CLEANLY:
      return NavigationEntryImpl::RESTORE_LAST_SESSION_EXITED_CLEANLY;
    case NavigationController::RESTORE_LAST_SESSION_CRASHED:
      return NavigationEntryImpl::RESTORE_LAST_SESSION_CRASHED;
  }
  NOTREACHED();
  return NavigationEntryImpl::RESTORE_CURRENT_SESSION;
}
