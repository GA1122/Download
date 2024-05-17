bool FileUtilProxy::Truncate(
    scoped_refptr<MessageLoopProxy> message_loop_proxy,
    PlatformFile file,
    int64 length,
    StatusCallback* callback) {
  return Start(FROM_HERE, message_loop_proxy,
               new RelayTruncatePlatformFile(file, length, callback));
}
