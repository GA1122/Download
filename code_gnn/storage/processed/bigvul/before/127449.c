void FileAPIMessageFilter::OnReadMetadata(
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
  operation->GetMetadata(
      url,
      base::Bind(&FileAPIMessageFilter::DidGetMetadata, this, request_id));
}
