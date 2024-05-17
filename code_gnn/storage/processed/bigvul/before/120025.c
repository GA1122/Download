void FileBrowserPrivateGetDriveFilesFunction::OnFileReady(
    drive::FileError error,
    const base::FilePath& local_path,
    scoped_ptr<drive::ResourceEntry> entry) {
  base::FilePath drive_path = remaining_drive_paths_.front();

  if (error == drive::FILE_ERROR_OK) {
    local_paths_.push_back(local_path.AsUTF8Unsafe());
    DVLOG(1) << "Got " << drive_path.value() << " as " << local_path.value();
  } else {
    local_paths_.push_back("");
    DVLOG(1) << "Failed to get " << drive_path.value()
             << " with error code: " << error;
  }

  remaining_drive_paths_.pop();

  GetFileOrSendResponse();
}
