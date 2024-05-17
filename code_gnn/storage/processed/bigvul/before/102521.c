  RelayCreateDirectory(
      const FilePath& file_path,
      bool exclusive,
      bool recursive,
      base::FileUtilProxy::StatusCallback* callback)
      : RelayWithStatusCallback(callback),
        file_path_(file_path),
        exclusive_(exclusive),
        recursive_(recursive) {
  }
