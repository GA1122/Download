OperationID FileSystemOperationRunner::Move(const FileSystemURL& src_url,
                                            const FileSystemURL& dest_url,
                                            CopyOrMoveOption option,
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
  PrepareForWrite(id, src_url);
  operation_raw->Move(src_url, dest_url, option,
                      base::BindOnce(&FileSystemOperationRunner::DidFinish,
                                     weak_ptr_, id, std::move(callback)));
  return id;
}
