void FileBrowserPrivateRequestDriveShareFunction::OnAddPermission(
    drive::FileError error) {
  SendResponse(error == drive::FILE_ERROR_OK);
}
