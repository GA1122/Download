void DownloadItemImpl::DeleteFile(const base::Callback<void(bool)>& callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  if (GetState() != DownloadItem::COMPLETE) {
    BrowserThread::PostTask(
        BrowserThread::UI, FROM_HERE,
        base::BindOnce(&DeleteDownloadedFileDone,
                       base::WeakPtr<DownloadItemImpl>(), callback, false));
    return;
  }
  if (GetFullPath().empty() || file_externally_removed_) {
    BrowserThread::PostTask(
        BrowserThread::UI, FROM_HERE,
        base::BindOnce(&DeleteDownloadedFileDone,
                       base::WeakPtr<DownloadItemImpl>(), callback, true));
    return;
  }
  base::PostTaskAndReplyWithResult(
      GetDownloadTaskRunner().get(), FROM_HERE,
      base::Bind(&DeleteDownloadedFile, GetFullPath()),
      base::Bind(&DeleteDownloadedFileDone, weak_ptr_factory_.GetWeakPtr(),
                 callback));
}
