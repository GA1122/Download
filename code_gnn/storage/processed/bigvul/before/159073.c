void ChromeDownloadManagerDelegate::ReserveVirtualPath(
    content::DownloadItem* download,
    const base::FilePath& virtual_path,
    bool create_directory,
    DownloadPathReservationTracker::FilenameConflictAction conflict_action,
    const DownloadTargetDeterminerDelegate::ReservedPathCallback& callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK(!virtual_path.empty());
#if defined(OS_CHROMEOS)
  if (drive::util::IsUnderDriveMountPoint(virtual_path)) {
    callback.Run(PathValidationResult::SUCCESS, virtual_path);
    return;
  }
#endif
  DownloadPathReservationTracker::GetReservedPath(
      download,
      virtual_path,
      download_prefs_->DownloadPath(),
      create_directory,
      conflict_action,
      callback);
}
