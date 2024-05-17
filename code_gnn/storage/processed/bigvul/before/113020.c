void DownloadItemImpl::Delete(DeleteReason reason) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  switch (reason) {
    case DELETE_DUE_TO_USER_DISCARD:
      UMA_HISTOGRAM_ENUMERATION(
          "Download.UserDiscard", GetDangerType(),
          content::DOWNLOAD_DANGER_TYPE_MAX);
      break;
    case DELETE_DUE_TO_BROWSER_SHUTDOWN:
      UMA_HISTOGRAM_ENUMERATION(
          "Download.Discard", GetDangerType(),
          content::DOWNLOAD_DANGER_TYPE_MAX);
      break;
    default:
      NOTREACHED();
  }

  if (!current_path_.empty())
    BrowserThread::PostTask(BrowserThread::FILE, FROM_HERE,
                            base::Bind(&DeleteDownloadedFile, current_path_));
  Remove();
}
