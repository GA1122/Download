bool FileUtilProxy::Flush(
    scoped_refptr<MessageLoopProxy> message_loop_proxy,
    PlatformFile file,
    StatusCallback* callback) {
  return Start(FROM_HERE, message_loop_proxy, new RelayFlush(file, callback));
}
