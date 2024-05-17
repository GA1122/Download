void DownloadItemImpl::OnAllDataSaved(
    int64 size, const std::string& final_hash) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  DCHECK(!all_data_saved_);
  all_data_saved_ = true;
  ProgressComplete(size, final_hash);
  UpdateObservers();
}
