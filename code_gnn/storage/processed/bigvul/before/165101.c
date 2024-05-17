FileSystemOperationRunner::FileSystemOperationRunner(
    FileSystemContext* file_system_context)
    : file_system_context_(file_system_context), weak_factory_(this) {
  weak_ptr_ = weak_factory_.GetWeakPtr();
}
