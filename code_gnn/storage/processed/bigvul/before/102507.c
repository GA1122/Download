bool FileUtilProxy::CreateTemporary(
    scoped_refptr<MessageLoopProxy> message_loop_proxy,
    int additional_file_flags,
    CreateTemporaryCallback* callback) {
  return Start(FROM_HERE, message_loop_proxy,
               new RelayCreateTemporary(message_loop_proxy,
                                        additional_file_flags,
                                        callback));
}
