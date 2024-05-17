void FileSystemOperationRunner::DidGetMetadata(
    const OperationID id,
    GetMetadataCallback callback,
    base::File::Error rv,
    const base::File::Info& file_info) {
  if (is_beginning_operation_) {
    finished_operations_.insert(id);
    base::ThreadTaskRunnerHandle::Get()->PostTask(
        FROM_HERE,
        base::BindOnce(&FileSystemOperationRunner::DidGetMetadata, weak_ptr_,
                       id, std::move(callback), rv, file_info));
    return;
  }
  std::move(callback).Run(rv, file_info);
  FinishOperation(id);
}
