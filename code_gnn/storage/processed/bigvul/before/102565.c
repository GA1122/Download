bool FileUtilProxy::Truncate(
    scoped_refptr<MessageLoopProxy> message_loop_proxy,
    const FilePath& path,
    int64 length,
    StatusCallback* callback) {
  return Start(FROM_HERE, message_loop_proxy,
               new RelayTruncate(path, length, callback));
}
