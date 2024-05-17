void GDataFileSystem::OnMoveEntryToDirectoryWithFileMoveCallback(
    const FileMoveCallback& callback,
    GDataFileError error,
    const FilePath& moved_file_path) {
  if (error == GDATA_FILE_OK)
    OnDirectoryChanged(moved_file_path.DirName());

  if (!callback.is_null())
    callback.Run(error, moved_file_path);
}
