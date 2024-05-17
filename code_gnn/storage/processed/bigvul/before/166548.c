void BrowserCommandController::OnSigninAllowedPrefChange() {
  if (!window())
    return;
  UpdateShowSyncState(IsShowingMainUI());
}
