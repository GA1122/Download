OperationID FileSystemOperationRunner::Copy(
    const FileSystemURL& src_url,
    const FileSystemURL& dest_url,
    CopyOrMoveOption option,
    ErrorBehavior error_behavior,
    const CopyProgressCallback& progress_callback,
    StatusCallback callback) {
  base::File::Error error = base::File::FILE_OK;
  std::unique_ptr<FileSystemOperation> operation = base::WrapUnique(
      file_system_context_->CreateFileSystemOperation(dest_url, &error));
  FileSystemOperation* operation_raw = operation.get();
  OperationID id = BeginOperation(std::move(operation));
  base::AutoReset<bool> beginning(&is_beginning_operation_, true);
  if (!operation_raw) {
    DidFinish(id, std::move(callback), error);
    return id;
  }
  PrepareForWrite(id, dest_url);
  PrepareForRead(id, src_url);
  operation_raw->Copy(
      src_url, dest_url, option, error_behavior,
      progress_callback.is_null()
          ? CopyProgressCallback()
          : base::BindRepeating(&FileSystemOperationRunner::OnCopyProgress,
                                weak_ptr_, id, progress_callback),
      base::BindOnce(&FileSystemOperationRunner::DidFinish, weak_ptr_, id,
                     std::move(callback)));
  return id;
}
