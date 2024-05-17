void FileSystemOperationRunner::DidReadDirectory(
    const OperationID id,
    const ReadDirectoryCallback& callback,
    base::File::Error rv,
    std::vector<filesystem::mojom::DirectoryEntry> entries,
    bool has_more) {
  if (is_beginning_operation_) {
    finished_operations_.insert(id);
    base::ThreadTaskRunnerHandle::Get()->PostTask(
        FROM_HERE,
        base::BindOnce(&FileSystemOperationRunner::DidReadDirectory, weak_ptr_,
                       id, callback, rv, std::move(entries), has_more));
    return;
  }
  callback.Run(rv, std::move(entries), has_more);
  if (rv != base::File::FILE_OK || !has_more)
    FinishOperation(id);
}
