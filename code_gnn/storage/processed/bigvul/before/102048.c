void SyncBackendHost::Core::DoShutdown(bool sync_disabled) {
  DCHECK(MessageLoop::current() == host_->sync_thread_.message_loop());

  save_changes_timer_.Stop();
  sync_manager_->Shutdown();   
  sync_manager_->RemoveObserver(this);
  sync_manager_.reset();
  registrar_->OnSyncerShutdownComplete();
  registrar_ = NULL;

  if (sync_disabled)
    DeleteSyncDataFolder();

  host_ = NULL;
}
