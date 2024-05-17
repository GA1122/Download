void DriveFileStreamReader::Initialize(
    const base::FilePath& drive_file_path,
    const InitializeCompletionCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  DCHECK(!callback.is_null());

  GetFileContentByPath(
      drive_file_system_getter_,
      drive_file_path,
      base::Bind(&DriveFileStreamReader
                     ::InitializeAfterGetFileContentByPathInitialized,
                 weak_ptr_factory_.GetWeakPtr(),
                 drive_file_path,
                 callback),
      base::Bind(&DriveFileStreamReader::OnGetContent,
                 weak_ptr_factory_.GetWeakPtr()),
      base::Bind(&DriveFileStreamReader::OnGetFileContentByPathCompletion,
                 weak_ptr_factory_.GetWeakPtr(),
                 callback));
}
