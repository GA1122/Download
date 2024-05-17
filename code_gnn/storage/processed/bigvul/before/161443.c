void StorageHandler::NotifyIndexedDBListChanged(const std::string& origin) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  frontend_->IndexedDBListUpdated(origin);
}
