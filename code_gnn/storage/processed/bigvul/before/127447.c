void FileAPIMessageFilter::OnOpenFile(
    int request_id, const GURL& path, int file_flags) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  base::PlatformFileError error;
  const int open_permissions = base::PLATFORM_FILE_OPEN |
                               (file_flags & kOpenFilePermissions);
  FileSystemURL url(path);
  if (!HasPermissionsForFile(url, open_permissions, &error)) {
    Send(new FileSystemMsg_DidFail(request_id, error));
    return;
  }

  FileSystemOperation* operation = GetNewOperation(url, request_id);
  if (!operation)
    return;
  operation->OpenFile(
      url, file_flags, peer_handle(),
      base::Bind(&FileAPIMessageFilter::DidOpenFile, this, request_id, path));
}
