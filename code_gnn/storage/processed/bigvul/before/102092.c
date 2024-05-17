void SyncBackendHost::Core::OnInitializationComplete(
    const WeakHandle<JsBackend>& js_backend,
    bool success) {
  if (!host_ || !host_->frontend_)
    return;   

  host_->frontend_loop_->PostTask(FROM_HERE,
      NewRunnableMethod(this,
                        &Core::HandleInitializationCompletedOnFrontendLoop,
                        js_backend, success));

  host_->sync_thread_.message_loop()->PostTask(FROM_HERE,
      NewRunnableMethod(this, &Core::StartSavingChanges));
}
