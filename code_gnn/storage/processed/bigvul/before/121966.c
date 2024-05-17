void AppListController::EnsureHaveKeepAliveForView() {
  if (!keep_alive_)
    keep_alive_.reset(new ScopedKeepAlive());
}
