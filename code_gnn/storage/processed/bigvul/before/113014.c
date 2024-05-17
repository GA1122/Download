void DownloadItemImpl::DangerousDownloadValidated() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK_EQ(DANGEROUS, GetSafetyState());

  UMA_HISTOGRAM_ENUMERATION("Download.DangerousDownloadValidated",
                            GetDangerType(),
                            content::DOWNLOAD_DANGER_TYPE_MAX);

  safety_state_ = DANGEROUS_BUT_VALIDATED;

  bound_net_log_.AddEvent(
      net::NetLog::TYPE_DOWNLOAD_ITEM_SAFETY_STATE_UPDATED,
      base::Bind(&download_net_logs::ItemCheckedCallback,
                 GetDangerType(), GetSafetyState()));

  UpdateObservers();

  delegate_->MaybeCompleteDownload(this);
}
