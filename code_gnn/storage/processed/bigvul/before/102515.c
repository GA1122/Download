static base::PlatformFileError PerformCommonCheckAndPreparationForMoveAndCopy(
    const FilePath& src_file_path,
    const FilePath& dest_file_path) {
  if (!file_util::PathExists(src_file_path))
    return base::PLATFORM_FILE_ERROR_NOT_FOUND;

  if (!file_util::DirectoryExists(dest_file_path.DirName()))
    return base::PLATFORM_FILE_ERROR_NOT_FOUND;

  if (src_file_path.IsParent(dest_file_path))
    return base::PLATFORM_FILE_ERROR_INVALID_OPERATION;

  if (!file_util::PathExists(dest_file_path))
    return base::PLATFORM_FILE_OK;

  bool src_is_directory = file_util::DirectoryExists(src_file_path);
  bool dest_is_directory = file_util::DirectoryExists(dest_file_path);
  if (src_is_directory && !dest_is_directory)
    return base::PLATFORM_FILE_ERROR_NOT_A_DIRECTORY;

  if (!src_is_directory && dest_is_directory)
    return base::PLATFORM_FILE_ERROR_NOT_A_FILE;

  if (src_file_path.value() == dest_file_path.value())
    return base::PLATFORM_FILE_ERROR_EXISTS;

  if (dest_is_directory) {
    if (!file_util::Delete(dest_file_path, false  )) {
      if (!file_util::IsDirectoryEmpty(dest_file_path))
        return base::PLATFORM_FILE_ERROR_NOT_EMPTY;
      return base::PLATFORM_FILE_ERROR_FAILED;
    }
  }
  return base::PLATFORM_FILE_OK;
}
