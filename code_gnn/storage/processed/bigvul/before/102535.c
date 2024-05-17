  RelayWrite(base::PlatformFile file,
             int64 offset,
             const char* buffer,
             int bytes_to_write,
             base::FileUtilProxy::WriteCallback* callback)
      : file_(file),
        offset_(offset),
        buffer_(new char[bytes_to_write]),
        bytes_to_write_(bytes_to_write),
        callback_(callback),
        bytes_written_(0) {
    memcpy(buffer_.get(), buffer, bytes_to_write);
  }
