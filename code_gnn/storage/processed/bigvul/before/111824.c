void SyncBackendHost::Core::OnInitializationComplete(
    const WeakHandle<JsBackend>& js_backend,
    bool success) {
  DCHECK_EQ(MessageLoop::current(), sync_loop_);
  host_.Call(
      FROM_HERE,
      &SyncBackendHost::HandleInitializationCompletedOnFrontendLoop,
      js_backend, success);

  if (success) {
    sync_loop_->PostTask(FROM_HERE,
                         base::Bind(&Core::StartSavingChanges, this));
  }
}
