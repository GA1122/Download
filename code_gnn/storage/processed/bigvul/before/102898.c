void TabCloseableStateWatcher::OnWindowCloseCanceled(Browser* browser) {
  if (signing_off_) {
    signing_off_ = false;
    CheckAndUpdateState(browser);
  }
}
