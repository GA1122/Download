void DownloadItemImpl::ValidateDangerousDownload() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK(!IsDone());
  DCHECK(IsDangerous());

  DVLOG(20) << __func__ << "() download=" << DebugString(true);

  if (IsDone() || !IsDangerous())
    return;

  RecordDangerousDownloadAccept(GetDangerType(),
                                GetTargetFilePath());

  danger_type_ = DOWNLOAD_DANGER_TYPE_USER_VALIDATED;

  TRACE_EVENT_INSTANT1("download", "DownloadItemSaftyStateUpdated",
                       TRACE_EVENT_SCOPE_THREAD, "danger_type",
                       GetDownloadDangerNames(danger_type_).c_str());

  UpdateObservers();   

  MaybeCompleteDownload();
}
