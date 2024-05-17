void FileAPIMessageFilter::OnCopy(
    int request_id, const GURL& src_path, const GURL& dest_path) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  base::PlatformFileError error;
  FileSystemURL src_url(src_path);
  FileSystemURL dest_url(dest_path);
  if (!HasPermissionsForFile(src_url, kReadFilePermissions, &error) ||
      !HasPermissionsForFile(dest_url, kCreateFilePermissions, &error)) {
    Send(new FileSystemMsg_DidFail(request_id, error));
    return;
  }

  FileSystemOperation* operation = GetNewOperation(src_url, request_id);
  if (!operation)
    return;
  operation->Copy(
      src_url, dest_url,
      base::Bind(&FileAPIMessageFilter::DidFinish, this, request_id));
}
