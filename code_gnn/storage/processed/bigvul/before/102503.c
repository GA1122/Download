bool FileUtilProxy::Close(scoped_refptr<MessageLoopProxy> message_loop_proxy,
                          base::PlatformFile file_handle,
                          StatusCallback* callback) {
  return Start(FROM_HERE, message_loop_proxy,
               new RelayClose(file_handle, callback));
}
