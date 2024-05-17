bool FileUtilProxy::CreateDirectory(
    scoped_refptr<MessageLoopProxy> message_loop_proxy,
    const FilePath& file_path,
    bool exclusive,
    bool recursive,
    StatusCallback* callback) {
  return Start(FROM_HERE, message_loop_proxy, new RelayCreateDirectory(
      file_path, exclusive, recursive, callback));
}
