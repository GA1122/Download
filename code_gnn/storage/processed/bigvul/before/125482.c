GDataFileSystem::~GDataFileSystem() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  feed_loader_->RemoveObserver(this);

  documents_service_->CancelAll();
}
