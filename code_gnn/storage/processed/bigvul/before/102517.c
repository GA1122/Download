bool FileUtilProxy::ReadDirectory(
    scoped_refptr<MessageLoopProxy> message_loop_proxy,
    const FilePath& file_path,
    ReadDirectoryCallback* callback) {
  return Start(FROM_HERE, message_loop_proxy, new RelayReadDirectory(
               file_path, callback));
}
