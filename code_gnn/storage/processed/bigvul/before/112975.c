void ChromeDownloadManagerDelegate::SubstituteGDataDownloadPathCallback(
    int32 download_id,
    bool should_prompt,
    bool is_forced_path,
    content::DownloadDangerType danger_type,
    const FilePath& suggested_path) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DownloadItem* download =
      download_manager_->GetActiveDownloadItem(download_id);
  if (!download)
    return;

  BrowserThread::PostTask(
      BrowserThread::FILE, FROM_HERE,
      base::Bind(&ChromeDownloadManagerDelegate::CheckIfSuggestedPathExists,
                 this, download->GetId(), suggested_path, should_prompt,
                 is_forced_path, danger_type,
                 download_prefs_->download_path()));
}
