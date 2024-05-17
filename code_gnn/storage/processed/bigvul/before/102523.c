  RelayCreateTemporary(
      scoped_refptr<base::MessageLoopProxy> message_loop_proxy,
      int additional_file_flags,
      base::FileUtilProxy::CreateTemporaryCallback* callback)
      : message_loop_proxy_(message_loop_proxy),
        additional_file_flags_(additional_file_flags),
        callback_(callback),
        file_handle_(base::kInvalidPlatformFileValue) {
    DCHECK(callback);
  }
