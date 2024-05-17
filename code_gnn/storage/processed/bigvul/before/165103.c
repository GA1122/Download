OperationID FileSystemOperationRunner::GetMetadata(
    const FileSystemURL& url,
    int fields,
    GetMetadataCallback callback) {
  base::File::Error error = base::File::FILE_OK;
  std::unique_ptr<FileSystemOperation> operation = base::WrapUnique(
      file_system_context_->CreateFileSystemOperation(url, &error));
  FileSystemOperation* operation_raw = operation.get();
  OperationID id = BeginOperation(std::move(operation));
  base::AutoReset<bool> beginning(&is_beginning_operation_, true);
  if (!operation_raw) {
    DidGetMetadata(id, std::move(callback), error, base::File::Info());
    return id;
  }
  PrepareForRead(id, url);
  operation_raw->GetMetadata(
      url, fields,
      base::BindOnce(&FileSystemOperationRunner::DidGetMetadata, weak_ptr_, id,
                     std::move(callback)));
  return id;
}
