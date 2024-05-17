  RelayClose(base::PlatformFile file_handle,
             base::FileUtilProxy::StatusCallback* callback)
      : RelayWithStatusCallback(callback),
        file_handle_(file_handle) {
  }
