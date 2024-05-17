oxide::qt::RestoreType ToInternalRestoreType(
    OxideQQuickWebView::RestoreType type) {
  switch (type) {
    case OxideQQuickWebView::RestoreCurrentSession:
      return oxide::qt::RESTORE_CURRENT_SESSION;
    case OxideQQuickWebView::RestoreLastSessionExitedCleanly:
      return oxide::qt::RESTORE_LAST_SESSION_EXITED_CLEANLY;
    case OxideQQuickWebView::RestoreLastSessionCrashed:
      return oxide::qt::RESTORE_LAST_SESSION_CRASHED;
  }
}
