void CancelGetFile(
    const DriveFileStreamReader::DriveFileSystemGetter& file_system_getter,
    const base::FilePath& drive_file_path) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  BrowserThread::PostTask(
      BrowserThread::UI,
      FROM_HERE,
      base::Bind(&CancelGetFileOnUIThread,
                 file_system_getter, drive_file_path));
}
