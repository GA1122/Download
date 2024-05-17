void FileAPIMessageFilter::OnReadDirectory(
    int request_id, const GURL& path) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  base::PlatformFileError error;
  FileSystemURL url(path);
  if (!HasPermissionsForFile(url, kReadFilePermissions, &error)) {
    Send(new FileSystemMsg_DidFail(request_id, error));
    return;
  }

  FileSystemOperation* operation = GetNewOperation(url, request_id);
  if (!operation)
    return;
  operation->ReadDirectory(
      url, base::Bind(&FileAPIMessageFilter::DidReadDirectory,
                       this, request_id));
}
