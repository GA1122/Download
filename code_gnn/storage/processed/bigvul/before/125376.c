void GetLocalFileSizeOnBlockingPool(const FilePath& local_file,
                                    GDataFileError* error,
                                    int64* file_size) {
  DCHECK(error);
  DCHECK(file_size);

  *file_size = 0;
  *error = file_util::GetFileSize(local_file, file_size) ?
      GDATA_FILE_OK :
      GDATA_FILE_ERROR_NOT_FOUND;
}
