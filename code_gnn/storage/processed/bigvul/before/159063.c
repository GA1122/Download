bool ChromeDownloadManagerDelegate::IsDownloadReadyForCompletion(
    DownloadItem* item,
    const base::Closure& internal_complete_callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
#if defined(FULL_SAFE_BROWSING)
  if (!download_prefs_->safebrowsing_for_trusted_sources_enabled() &&
      download_prefs_->IsFromTrustedSource(*item)) {
    return true;
  }

  SafeBrowsingState* state = static_cast<SafeBrowsingState*>(
      item->GetUserData(&kSafeBrowsingUserDataKey));
  if (!state) {
    DownloadProtectionService* service = GetDownloadProtectionService();
    if (service) {
      DVLOG(2) << __func__ << "() Start SB download check for download = "
               << item->DebugString(false);
      state = new SafeBrowsingState();
      state->set_callback(internal_complete_callback);
      item->SetUserData(&kSafeBrowsingUserDataKey, base::WrapUnique(state));
      service->CheckClientDownload(
          item,
          base::Bind(&ChromeDownloadManagerDelegate::CheckClientDownloadDone,
                     weak_ptr_factory_.GetWeakPtr(),
                     item->GetId()));
      return false;
    }

    content::DownloadDangerType danger_type = item->GetDangerType();
    if (DownloadItemModel(item).GetDangerLevel() !=
            DownloadFileType::NOT_DANGEROUS &&
        (danger_type == content::DOWNLOAD_DANGER_TYPE_NOT_DANGEROUS ||
         danger_type ==
             content::DOWNLOAD_DANGER_TYPE_MAYBE_DANGEROUS_CONTENT)) {
      DVLOG(2) << __func__
               << "() SB service disabled. Marking download as DANGEROUS FILE";
      if (ShouldBlockFile(content::DOWNLOAD_DANGER_TYPE_DANGEROUS_FILE)) {
        item->OnContentCheckCompleted(
            content::DOWNLOAD_DANGER_TYPE_NOT_DANGEROUS,
            content::DOWNLOAD_INTERRUPT_REASON_FILE_BLOCKED);
      } else {
        item->OnContentCheckCompleted(
            content::DOWNLOAD_DANGER_TYPE_DANGEROUS_FILE,
            content::DOWNLOAD_INTERRUPT_REASON_NONE);
      }
      UMA_HISTOGRAM_ENUMERATION("Download.DangerousFile.Reason",
                                SB_NOT_AVAILABLE, DANGEROUS_FILE_REASON_MAX);
      content::BrowserThread::PostTask(content::BrowserThread::UI, FROM_HERE,
                                       internal_complete_callback);
      return false;
    }
  } else if (!state->is_complete()) {
    state->set_callback(internal_complete_callback);
    return false;
  }

#endif
  return true;
}
