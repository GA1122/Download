void CancelGetFileOnUIThread(
    const DriveFileStreamReader::DriveFileSystemGetter& file_system_getter,
    const base::FilePath& drive_file_path) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  DriveFileSystemInterface* file_system = file_system_getter.Run();
  if (file_system) {
    file_system->CancelGetFile(drive_file_path);
  }
}
