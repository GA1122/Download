void ChromeDownloadManagerDelegate::SetNextId(uint32_t next_id) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK(!profile_->IsOffTheRecord());

  bool download_db_available = (next_id != content::DownloadItem::kInvalidId);
  RecordDatabaseAvailability(download_db_available);
  if (download_db_available) {
    next_download_id_ = next_id;
  } else {
    next_download_id_ = content::DownloadItem::kInvalidId + 1;
  }

  IdCallbackVector callbacks;
  id_callbacks_.swap(callbacks);
  for (IdCallbackVector::const_iterator it = callbacks.begin();
       it != callbacks.end(); ++it) {
    ReturnNextId(*it);
  }
}
