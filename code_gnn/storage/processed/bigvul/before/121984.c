bool AppListController::IsWarmupNeeded() {
  if (!g_browser_process || g_browser_process->IsShuttingDown())
    return false;

  return !current_view_ && !profile_loader().IsAnyProfileLoading();
}
