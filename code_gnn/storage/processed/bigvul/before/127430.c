FileSystemOperation* FileAPIMessageFilter::GetNewOperation(
    const FileSystemURL& target_url,
    int request_id) {
  base::PlatformFileError error_code;
  FileSystemOperation* operation =
      context_->CreateFileSystemOperation(target_url, &error_code);
  if (error_code != base::PLATFORM_FILE_OK) {
    Send(new FileSystemMsg_DidFail(request_id, error_code));
    return NULL;
  }

  DCHECK(operation);
  operations_.AddWithID(operation, request_id);
  return operation;
}
