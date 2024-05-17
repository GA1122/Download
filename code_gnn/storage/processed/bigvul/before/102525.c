  RelayEnsureFileExists(
      scoped_refptr<base::MessageLoopProxy> message_loop_proxy,
      const FilePath& file_path,
      base::FileUtilProxy::EnsureFileExistsCallback* callback)
      : message_loop_proxy_(message_loop_proxy),
        file_path_(file_path),
        callback_(callback),
        created_(false) {
    DCHECK(callback);
  }
