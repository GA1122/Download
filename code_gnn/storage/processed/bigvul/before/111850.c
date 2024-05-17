SyncBackendHost::Core::~Core() {
  DCHECK(!sync_manager_.get());
  DCHECK(!sync_loop_);
}
