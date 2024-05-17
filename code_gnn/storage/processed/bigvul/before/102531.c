  RelayTouchFilePath(const FilePath& file_path,
                     const base::Time& last_access_time,
                     const base::Time& last_modified_time,
                     base::FileUtilProxy::StatusCallback* callback)
      : RelayWithStatusCallback(callback),
        file_path_(file_path),
        last_access_time_(last_access_time),
        last_modified_time_(last_modified_time) {
  }
