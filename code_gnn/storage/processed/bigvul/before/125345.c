void CopyLocalFileOnBlockingPool(
    const FilePath& src_file_path,
    const FilePath& dest_file_path,
    GDataFileError* error) {
  DCHECK(error);

  *error = file_util::CopyFile(src_file_path, dest_file_path) ?
      GDATA_FILE_OK : GDATA_FILE_ERROR_FAILED;
}
