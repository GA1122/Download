bool FileUtilProxy::EnsureFileExists(
    scoped_refptr<MessageLoopProxy> message_loop_proxy,
    const FilePath& file_path,
    EnsureFileExistsCallback* callback) {
  return Start(FROM_HERE, message_loop_proxy, new RelayEnsureFileExists(
      message_loop_proxy, file_path, callback));
}
