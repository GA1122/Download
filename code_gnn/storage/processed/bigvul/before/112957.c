bool ChromeDownloadManagerDelegate::IsDangerousFile(
    const DownloadItem& download,
    const FilePath& suggested_path,
    bool visited_referrer_before) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (download.GetTransitionType() & content::PAGE_TRANSITION_FROM_ADDRESS_BAR)
    return false;

  if (extensions::switch_utils::IsEasyOffStoreInstallEnabled() &&
      download_crx_util::IsExtensionDownload(download) &&
      !WebstoreInstaller::GetAssociatedApproval(download)) {
    return true;
  }

  if (ShouldOpenFileBasedOnExtension(suggested_path) &&
      download.HasUserGesture())
    return false;

  download_util::DownloadDangerLevel danger_level =
      download_util::GetFileDangerLevel(suggested_path.BaseName());
  if (danger_level == download_util::AllowOnUserGesture)
    return !download.HasUserGesture() || !visited_referrer_before;

  return danger_level == download_util::Dangerous;
}
