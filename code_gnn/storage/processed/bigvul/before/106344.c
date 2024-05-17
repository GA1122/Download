void SyncBackendHost::Core::OnInitializationComplete() {
  if (!host_ || !host_->frontend_)
    return;   

  host_->frontend_loop_->PostTask(FROM_HERE,
      NewRunnableMethod(this,
                        &Core::HandleInitalizationCompletedOnFrontendLoop));

  host_->core_thread_.message_loop()->PostTask(FROM_HERE,
      NewRunnableMethod(this, &Core::StartSavingChanges));
}
