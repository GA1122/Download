  RelayGetFileInfo(const FilePath& file_path,
                   base::FileUtilProxy::GetFileInfoCallback* callback)
      : callback_(callback),
        file_path_(file_path) {
    DCHECK(callback);
  }
