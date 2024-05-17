  RelayCopy(const FilePath& src_file_path,
            const FilePath& dest_file_path,
            base::FileUtilProxy::StatusCallback* callback)
      : RelayWithStatusCallback(callback),
        src_file_path_(src_file_path),
        dest_file_path_(dest_file_path) {
  }
