  RelayRead(base::PlatformFile file,
            int64 offset,
            int bytes_to_read,
            base::FileUtilProxy::ReadCallback* callback)
      : file_(file),
        offset_(offset),
        buffer_(new char[bytes_to_read]),
        bytes_to_read_(bytes_to_read),
        callback_(callback),
        bytes_read_(0) {
  }
