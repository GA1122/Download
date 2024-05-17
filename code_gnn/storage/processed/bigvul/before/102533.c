  RelayTruncatePlatformFile(base::PlatformFile file,
                            int64 length,
                            base::FileUtilProxy::StatusCallback* callback)
      : RelayWithStatusCallback(callback),
        file_(file),
        length_(length) {
  }
