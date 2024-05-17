DataObjectImpl::~DataObjectImpl() {
  StopDownloads();
  if (observer_)
    observer_->OnDataObjectDisposed();
}
