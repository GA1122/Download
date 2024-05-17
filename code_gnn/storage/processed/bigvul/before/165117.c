OperationID FileSystemOperationRunner::Write(
    const FileSystemURL& url,
    mojo::ScopedDataPipeConsumerHandle data_pipe,
    int64_t offset,
    const WriteCallback& callback) {
  base::File::Error error = base::File::FILE_OK;
  std::unique_ptr<FileSystemOperation> operation = base::WrapUnique(
      file_system_context_->CreateFileSystemOperation(url, &error));
  FileSystemOperation* operation_raw = operation.get();
  OperationID id = BeginOperation(std::move(operation));
  base::AutoReset<bool> beginning(&is_beginning_operation_, true);
  if (!operation_raw) {
    DidWrite(id, callback, error, 0, true);
    return id;
  }

  std::unique_ptr<FileStreamWriter> writer(
      file_system_context_->CreateFileStreamWriter(url, offset));
  if (!writer) {
    DidWrite(id, callback, base::File::FILE_ERROR_SECURITY, 0, true);
    return id;
  }

  std::unique_ptr<FileWriterDelegate> writer_delegate(new FileWriterDelegate(
      std::move(writer), url.mount_option().flush_policy()));

  PrepareForWrite(id, url);
  operation_raw->Write(url, std::move(writer_delegate), std::move(data_pipe),
                       base::BindRepeating(&FileSystemOperationRunner::DidWrite,
                                           weak_ptr_, id, callback));
  return id;
}
