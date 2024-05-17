void FileSystemOperationRunner::DidCreateSnapshot(
    const OperationID id,
    SnapshotFileCallback callback,
    base::File::Error rv,
    const base::File::Info& file_info,
    const base::FilePath& platform_path,
    scoped_refptr<storage::ShareableFileReference> file_ref) {
  if (is_beginning_operation_) {
    finished_operations_.insert(id);
    base::ThreadTaskRunnerHandle::Get()->PostTask(
        FROM_HERE,
        base::BindOnce(&FileSystemOperationRunner::DidCreateSnapshot, weak_ptr_,
                       id, std::move(callback), rv, file_info, platform_path,
                       std::move(file_ref)));
    return;
  }
  std::move(callback).Run(rv, file_info, platform_path, std::move(file_ref));
  FinishOperation(id);
}
