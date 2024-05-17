void OnTransferRegularFileCompleteForCopy(
    const FileOperationCallback& callback,
    scoped_refptr<base::MessageLoopProxy> relay_proxy,
    GDataFileError error) {
  if (!callback.is_null())
    relay_proxy->PostTask(FROM_HERE, base::Bind(callback, error));
}
