void DatabaseMessageFilter::AddObserver() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::FILE));
  db_tracker_->AddObserver(this);
}
