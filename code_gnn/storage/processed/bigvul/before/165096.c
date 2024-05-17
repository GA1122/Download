void FileSystemOperationRunner::DidOpenFile(
    const OperationID id,
    OpenFileCallback callback,
    base::File file,
    base::OnceClosure on_close_callback) {
  if (is_beginning_operation_) {
    finished_operations_.insert(id);
    base::ThreadTaskRunnerHandle::Get()->PostTask(
        FROM_HERE,
        base::BindOnce(&FileSystemOperationRunner::DidOpenFile, weak_ptr_, id,
                       std::move(callback), std::move(file),
                       std::move(on_close_callback)));
    return;
  }
  std::move(callback).Run(std::move(file), std::move(on_close_callback));
  FinishOperation(id);
}
