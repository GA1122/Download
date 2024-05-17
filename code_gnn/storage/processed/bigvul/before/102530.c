  RelayReadDirectory(const FilePath& file_path,
      base::FileUtilProxy::ReadDirectoryCallback* callback)
      : callback_(callback), file_path_(file_path) {
    DCHECK(callback);
  }
