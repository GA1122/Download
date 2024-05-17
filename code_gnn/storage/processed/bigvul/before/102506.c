bool FileUtilProxy::CreateOrOpen(
    scoped_refptr<MessageLoopProxy> message_loop_proxy,
    const FilePath& file_path, int file_flags,
    CreateOrOpenCallback* callback) {
  return Start(FROM_HERE, message_loop_proxy, new RelayCreateOrOpen(
      message_loop_proxy, file_path, file_flags, callback));
}
