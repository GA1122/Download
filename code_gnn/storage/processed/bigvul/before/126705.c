bool UnloadController::ShouldCloseWindow() {
  if (HasCompletedUnloadProcessing())
    return true;

  is_attempting_to_close_browser_ = true;

  if (!TabsNeedBeforeUnloadFired())
    return true;

  ProcessPendingTabs();
  return false;
}
