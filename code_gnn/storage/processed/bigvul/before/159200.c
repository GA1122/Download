void DownloadItemImpl::StealDangerousDownload(
    bool delete_file_afterward,
    const AcquireFileCallback& callback) {
  DVLOG(20) << __func__ << "() download = " << DebugString(true);
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK(IsDangerous());
  DCHECK(AllDataSaved());

  if (delete_file_afterward) {
    if (download_file_) {
      base::PostTaskAndReplyWithResult(
          GetDownloadTaskRunner().get(), FROM_HERE,
          base::Bind(&DownloadFileDetach, base::Passed(&download_file_)),
          callback);
    } else {
      callback.Run(GetFullPath());
    }
    destination_info_.current_path.clear();
    Remove();
  } else if (download_file_) {
    base::PostTaskAndReplyWithResult(
        GetDownloadTaskRunner().get(), FROM_HERE,
        base::Bind(&MakeCopyOfDownloadFile, download_file_.get()), callback);
  } else {
    callback.Run(GetFullPath());
  }
}
