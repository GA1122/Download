base::File::Error FileSystemOperationRunner::SyncGetPlatformPath(
    const FileSystemURL& url,
    base::FilePath* platform_path) {
  base::File::Error error = base::File::FILE_OK;
  std::unique_ptr<FileSystemOperation> operation(
      file_system_context_->CreateFileSystemOperation(url, &error));
  if (!operation.get())
    return error;
  return operation->SyncGetPlatformPath(url, platform_path);
}
