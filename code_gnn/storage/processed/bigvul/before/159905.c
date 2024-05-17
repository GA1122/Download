void TopSitesImpl::ShutdownOnUIThread() {
  history_service_ = nullptr;
  history_service_observer_.RemoveAll();
  cancelable_task_tracker_.TryCancelAll();
  if (backend_)
    backend_->Shutdown();
}
