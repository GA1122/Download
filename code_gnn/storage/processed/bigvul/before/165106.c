void FileSystemOperationRunner::OnCopyProgress(
    const OperationID id,
    const CopyProgressCallback& callback,
    FileSystemOperation::CopyProgressType type,
    const FileSystemURL& source_url,
    const FileSystemURL& dest_url,
    int64_t size) {
  if (is_beginning_operation_) {
    base::ThreadTaskRunnerHandle::Get()->PostTask(
        FROM_HERE,
        base::BindOnce(&FileSystemOperationRunner::OnCopyProgress, weak_ptr_,
                       id, callback, type, source_url, dest_url, size));
    return;
  }
  callback.Run(type, source_url, dest_url, size);
}
