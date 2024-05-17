void GetFileContentByPathOnUIThread(
    const DriveFileStreamReader::DriveFileSystemGetter& file_system_getter,
    const base::FilePath& drive_file_path,
    const GetFileContentInitializedCallback& initialized_callback,
    const google_apis::GetContentCallback& get_content_callback,
    const FileOperationCallback& completion_callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  DriveFileSystemInterface* file_system = file_system_getter.Run();
  if (!file_system) {
    completion_callback.Run(FILE_ERROR_FAILED);
    return;
  }

  file_system->GetFileContentByPath(drive_file_path,
                                    initialized_callback,
                                    get_content_callback,
                                    completion_callback);
}
