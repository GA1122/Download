DownloadItemImpl::~DownloadItemImpl() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  TransitionTo(REMOVING);
  STLDeleteContainerPairSecondPointers(
      external_data_map_.begin(), external_data_map_.end());
  delegate_->AssertStateConsistent(this);
  delegate_->Detach();
}
