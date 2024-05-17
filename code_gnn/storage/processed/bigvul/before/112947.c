void ChromeDownloadManagerDelegate::CheckVisitedReferrerBeforeDone(
    int32 download_id,
    content::DownloadDangerType danger_type,
    bool visited_referrer_before) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  DownloadItem* download =
      download_manager_->GetActiveDownloadItem(download_id);
  if (!download)
    return;

  bool should_prompt = (download->GetTargetDisposition() ==
                        DownloadItem::TARGET_DISPOSITION_PROMPT);
  bool is_forced_path = !download->GetForcedFilePath().empty();
  FilePath suggested_path;

  if (!is_forced_path) {
    FilePath generated_name;
    download_util::GenerateFileNameFromRequest(*download, &generated_name);

    if (download_prefs_->PromptForDownload()) {
      if (!download_crx_util::IsExtensionDownload(*download) &&
          !ShouldOpenFileBasedOnExtension(generated_name))
        should_prompt = true;
    }
    if (download_prefs_->IsDownloadPathManaged())
      should_prompt = false;

    FilePath target_directory;
    if (should_prompt && !download_manager_->LastDownloadPath().empty())
      target_directory = download_manager_->LastDownloadPath();
    else
      target_directory = download_prefs_->download_path();
    suggested_path = target_directory.Append(generated_name);
  } else {
    DCHECK(!should_prompt);
    suggested_path = download->GetForcedFilePath();
  }

  if (danger_type == content::DOWNLOAD_DANGER_TYPE_NOT_DANGEROUS) {
    if (!should_prompt && !is_forced_path &&
        IsDangerousFile(*download, suggested_path, visited_referrer_before)) {
      danger_type = content::DOWNLOAD_DANGER_TYPE_DANGEROUS_FILE;
    }

#if defined(ENABLE_SAFE_BROWSING)
    DownloadProtectionService* service = GetDownloadProtectionService();
    if (service && service->enabled()) {
      DownloadProtectionService::DownloadInfo info =
          DownloadProtectionService::DownloadInfo::FromDownloadItem(*download);
      info.target_file = suggested_path;
      if (service->IsSupportedDownload(info))
        danger_type = content::DOWNLOAD_DANGER_TYPE_MAYBE_DANGEROUS_CONTENT;
    }
#endif
  } else {
    DCHECK_EQ(content::DOWNLOAD_DANGER_TYPE_DANGEROUS_URL, danger_type);
  }

#if defined (OS_CHROMEOS)
  gdata::GDataDownloadObserver::SubstituteGDataDownloadPath(
      profile_, suggested_path, download,
      base::Bind(
          &ChromeDownloadManagerDelegate::SubstituteGDataDownloadPathCallback,
          this, download->GetId(), should_prompt, is_forced_path, danger_type));
#else
  BrowserThread::PostTask(
      BrowserThread::FILE, FROM_HERE,
      base::Bind(&ChromeDownloadManagerDelegate::CheckIfSuggestedPathExists,
                 this, download->GetId(), suggested_path, should_prompt,
                 is_forced_path, danger_type,
                 download_prefs_->download_path()));
#endif
}
