void SyncBackendHost::Core::HandleInitializationCompletedOnFrontendLoop(
    const WeakHandle<JsBackend>& js_backend,
    bool success) {
  if (!host_)
    return;
  host_->HandleInitializationCompletedOnFrontendLoop(js_backend, success);
}
