 Response StorageHandler::Disable() {
  if (cache_storage_observer_) {
    BrowserThread::DeleteSoon(BrowserThread::IO, FROM_HERE,
                              cache_storage_observer_.release());
  }
  if (indexed_db_observer_) {
    scoped_refptr<base::SequencedTaskRunner> observer_task_runner =
        indexed_db_observer_->TaskRunner();
    observer_task_runner->DeleteSoon(FROM_HERE,
                                     std::move(indexed_db_observer_));
  }

  return Response::OK();
}
