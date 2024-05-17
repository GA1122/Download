bool FileUtilProxy::Delete(scoped_refptr<MessageLoopProxy> message_loop_proxy,
                           const FilePath& file_path,
                           bool recursive,
                           StatusCallback* callback) {
  return Start(FROM_HERE, message_loop_proxy,
               new RelayDelete(file_path, recursive, callback));
}
