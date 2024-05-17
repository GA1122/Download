OperationID FileSystemOperationRunner::CreateSnapshotFile(
    const FileSystemURL& url,
    SnapshotFileCallback callback) {
  base::File::Error error = base::File::FILE_OK;
  std::unique_ptr<FileSystemOperation> operation = base::WrapUnique(
      file_system_context_->CreateFileSystemOperation(url, &error));
  FileSystemOperation* operation_raw = operation.get();
  OperationID id = BeginOperation(std::move(operation));
  base::AutoReset<bool> beginning(&is_beginning_operation_, true);
  if (!operation_raw) {
    DidCreateSnapshot(id, std::move(callback), error, base::File::Info(),
                      base::FilePath(), nullptr);
    return id;
  }
  PrepareForRead(id, url);
  operation_raw->CreateSnapshotFile(
      url, base::BindOnce(&FileSystemOperationRunner::DidCreateSnapshot,
                          weak_ptr_, id, std::move(callback)));
  return id;
}
