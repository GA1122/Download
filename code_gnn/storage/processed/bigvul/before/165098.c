void FileSystemOperationRunner::DidWrite(const OperationID id,
                                         const WriteCallback& callback,
                                         base::File::Error rv,
                                         int64_t bytes,
                                         bool complete) {
  if (is_beginning_operation_) {
    finished_operations_.insert(id);
    base::ThreadTaskRunnerHandle::Get()->PostTask(
        FROM_HERE,
        base::BindOnce(&FileSystemOperationRunner::DidWrite, weak_ptr_, id,
                       callback, rv, bytes, complete));
    return;
  }
  callback.Run(rv, bytes, complete);
  if (rv != base::File::FILE_OK || complete)
    FinishOperation(id);
}
