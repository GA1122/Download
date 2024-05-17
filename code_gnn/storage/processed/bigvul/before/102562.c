bool FileUtilProxy::Touch(
    scoped_refptr<MessageLoopProxy> message_loop_proxy,
    PlatformFile file,
    const base::Time& last_access_time,
    const base::Time& last_modified_time,
    StatusCallback* callback) {
  return Start(FROM_HERE, message_loop_proxy,
               new RelayTouch(file, last_access_time, last_modified_time,
                              callback));
}
