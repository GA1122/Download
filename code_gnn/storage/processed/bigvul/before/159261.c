void DownloadManagerImpl::OnFileExistenceChecked(uint32_t download_id,
                                                 bool result) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  if (!result) {   
    if (base::ContainsKey(downloads_, download_id))
      downloads_[download_id]->OnDownloadedFileRemoved();
  }
}