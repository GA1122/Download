  RelayDelete(const FilePath& file_path,
              bool recursive,
              base::FileUtilProxy::StatusCallback* callback)
      : RelayWithStatusCallback(callback),
        file_path_(file_path),
        recursive_(recursive) {
  }
