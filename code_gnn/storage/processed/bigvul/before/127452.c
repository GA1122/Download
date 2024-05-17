void FileAPIMessageFilter::OnSyncGetPlatformPath(
    const GURL& path, FilePath* platform_path) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::FILE));
  DCHECK(platform_path);
  *platform_path = FilePath();
  FileSystemURL url(path);
  if (!url.is_valid())
    return;

  base::PlatformFileError error;
  if (!HasPermissionsForFile(url, kReadFilePermissions, &error))
    return;

  LocalFileSystemOperation* operation =
      context_->CreateFileSystemOperation(
          url, NULL)->AsLocalFileSystemOperation();
  DCHECK(operation);
  if (!operation)
    return;

  operation->SyncGetPlatformPath(url, platform_path);

  if (!ChildProcessSecurityPolicyImpl::GetInstance()->CanReadFile(
          process_id_, *platform_path)) {
    ChildProcessSecurityPolicyImpl::GetInstance()->GrantReadFile(
        process_id_, *platform_path);
  }
}
