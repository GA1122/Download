bool FileUtilProxy::GetFileInfoFromPlatformFile(
    scoped_refptr<MessageLoopProxy> message_loop_proxy,
    PlatformFile file,
    GetFileInfoCallback* callback) {
  return Start(FROM_HERE, message_loop_proxy,
               new RelayGetFileInfoFromPlatformFile(file, callback));
}
