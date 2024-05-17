  CacheStorageObserver(base::WeakPtr<StorageHandler> owner_storage_handler,
                       CacheStorageContextImpl* cache_storage_context)
      : owner_(owner_storage_handler), context_(cache_storage_context) {
    BrowserThread::PostTask(
        BrowserThread::IO, FROM_HERE,
        base::BindOnce(&CacheStorageObserver::AddObserverOnIOThread,
                       base::Unretained(this)));
  }
