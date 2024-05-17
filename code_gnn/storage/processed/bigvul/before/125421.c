void GDataFileSystem::OnMoveEntryToDirectoryWithFileOperationCallback(
    const FileOperationCallback& callback,
    GDataFileError error,
    const FilePath& moved_file_path) {
  DCHECK(!callback.is_null());

  if (error == GDATA_FILE_OK)
    OnDirectoryChanged(moved_file_path.DirName());

  callback.Run(error);
}
