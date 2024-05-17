  IndexedDBObserver(base::WeakPtr<StorageHandler> owner_storage_handler,
                    IndexedDBContextImpl* indexed_db_context)
      : owner_(owner_storage_handler), context_(indexed_db_context) {
    context_->TaskRunner()->PostTask(
        FROM_HERE, base::BindOnce(&IndexedDBObserver::AddObserverOnIDBThread,
                                  base::Unretained(this)));
  }
