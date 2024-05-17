void DownloadFileManager::CancelDownload(DownloadId global_id) {
  VLOG(20) << __FUNCTION__ << "()" << " id = " << global_id;
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::FILE));
  DownloadFileMap::iterator it = downloads_.find(global_id);
  if (it == downloads_.end())
    return;

  DownloadFile* download_file = it->second;
  VLOG(20) << __FUNCTION__ << "()"
           << " download_file = " << download_file->DebugString();
  download_file->Cancel();

   EraseDownload(global_id);
 }
