void ChromeDownloadManagerDelegate::CheckForFileExistence(
    DownloadItem* download,
    content::CheckForFileExistenceCallback callback) {
#if defined(OS_CHROMEOS)
  drive::DownloadHandler* drive_download_handler =
      drive::DownloadHandler::GetForProfile(profile_);
  if (drive_download_handler &&
      drive_download_handler->IsDriveDownload(download)) {
    drive_download_handler->CheckForFileExistence(download,
                                                  std::move(callback));
    return;
  }
#endif
  base::PostTaskAndReplyWithResult(
      disk_access_task_runner_.get(), FROM_HERE,
      base::BindOnce(&base::PathExists, download->GetTargetFilePath()),
      std::move(callback));
}
