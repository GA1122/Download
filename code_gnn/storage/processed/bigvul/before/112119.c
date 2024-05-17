void SyncManager::SyncInternal::ShutdownOnSyncThread() {
  DCHECK(thread_checker_.CalledOnValidThread());

  weak_ptr_factory_.InvalidateWeakPtrs();
  js_mutation_event_observer_.InvalidateWeakPtrs();

  scheduler_.reset();

  SetJsEventHandler(WeakHandle<JsEventHandler>());
  RemoveObserver(&js_sync_manager_observer_);

  RemoveObserver(&debug_info_event_listener_);

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

  if (initialized_ && directory()) {
    {
      ReadTransaction trans(FROM_HERE, GetUserShare());
      trans.GetCryptographer()->RemoveObserver(this);
    }
    directory()->SaveChanges();
  }

  share_.directory.reset();

  change_delegate_ = NULL;
  registrar_ = NULL;

  initialized_ = false;

  change_observer_.Reset();
  weak_handle_this_.Reset();
}
