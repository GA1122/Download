bool Browser::ShouldCloseWindow() {
  if (!CanCloseWithInProgressDownloads())
    return false;

  if (HasCompletedUnloadProcessing())
    return IsClosingPermitted();

  is_attempting_to_close_browser_ = true;

  if (!TabsNeedBeforeUnloadFired())
    return IsClosingPermitted();

  ProcessPendingTabs();
  return false;
}
