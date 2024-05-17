bool FileUtilProxy::Copy(scoped_refptr<MessageLoopProxy> message_loop_proxy,
                         const FilePath& src_file_path,
                         const FilePath& dest_file_path,
                         StatusCallback* callback) {
  return Start(FROM_HERE, message_loop_proxy,
               new RelayCopy(src_file_path, dest_file_path, callback));
}
