void SyncManager::SyncInternal::Shutdown() {
  DCHECK(thread_checker_.CalledOnValidThread());

  weak_ptr_factory_.InvalidateWeakPtrs();

  scheduler_.reset();

  SetJsEventHandler(WeakHandle<JsEventHandler>());
  RemoveObserver(&js_sync_manager_observer_);

  if (sync_notifier_.get()) {
    sync_notifier_->RemoveObserver(this);
  }
  sync_notifier_.reset();

  if (connection_manager_.get()) {
    connection_manager_->RemoveListener(this);
  }
  connection_manager_.reset();

  net::NetworkChangeNotifier::RemoveIPAddressObserver(this);
  observing_ip_address_changes_ = false;

  if (dir_manager()) {
    syncable::ScopedDirLookup lookup(dir_manager(), username_for_share());
    if (lookup.good()) {
      lookup->RemoveTransactionObserver(&js_transaction_observer_);
    } else {
      NOTREACHED();
    }
    dir_manager()->FinalSaveChangesForAll();
    dir_manager()->Close(username_for_share());
  }

  share_.dir_manager.reset();

  setup_for_test_mode_ = false;
  registrar_ = NULL;

  initialized_ = false;

  weak_handle_this_.Reset();
}
