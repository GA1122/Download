void FileBrowserPrivateGetDriveFilesFunction::GetFileOrSendResponse() {
  if (remaining_drive_paths_.empty()) {
    results_ = extensions::api::file_browser_private::
        GetDriveFiles::Results::Create(local_paths_);
    SendResponse(true);
    return;
  }

  base::FilePath drive_path = remaining_drive_paths_.front();

  drive::FileSystemInterface* file_system =
      drive::util::GetFileSystemByProfile(GetProfile());
  if (!file_system) {
    OnFileReady(drive::FILE_ERROR_FAILED, drive_path,
                scoped_ptr<drive::ResourceEntry>());
    return;
  }

  file_system->GetFile(
      drive_path,
      base::Bind(&FileBrowserPrivateGetDriveFilesFunction::OnFileReady, this));
}
