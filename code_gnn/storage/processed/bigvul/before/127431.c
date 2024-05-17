bool FileAPIMessageFilter::HasPermissionsForFile(
    const FileSystemURL& url, int permissions, base::PlatformFileError* error) {
  DCHECK(error);
  *error = base::PLATFORM_FILE_OK;

  if (!url.is_valid()) {
    *error = base::PLATFORM_FILE_ERROR_INVALID_URL;
    return false;
  }

  FileSystemMountPointProvider* mount_point_provider =
      context_->GetMountPointProvider(url.type());
  if (!mount_point_provider) {
    *error = base::PLATFORM_FILE_ERROR_INVALID_URL;
    return false;
  }

  FilePath file_path;
  ChildProcessSecurityPolicyImpl* policy =
      ChildProcessSecurityPolicyImpl::GetInstance();

  if (url.mount_type() == fileapi::kFileSystemTypeIsolated) {
    if (url.type() == fileapi::kFileSystemTypeDragged && url.path().empty()) {
      if (permissions != kReadFilePermissions) {
        *error = base::PLATFORM_FILE_ERROR_SECURITY;
        return false;
      }
      return true;
    }

    bool success = policy->HasPermissionsForFileSystem(
        process_id_, url.filesystem_id(), permissions);
    if (!success)
      *error = base::PLATFORM_FILE_ERROR_SECURITY;
    return success;
  }

  if (fileapi::SandboxMountPointProvider::CanHandleType(url.type())) {
    CHECK(mount_point_provider == context_->sandbox_provider());
    return true;
  }

  file_path = mount_point_provider->GetPathForPermissionsCheck(url.path());
  if (file_path.empty()) {
    *error = base::PLATFORM_FILE_ERROR_SECURITY;
    return false;
  }

  bool success = policy->HasPermissionsForFile(
      process_id_, file_path, permissions);
  if (!success)
    *error = base::PLATFORM_FILE_ERROR_SECURITY;
  return success;
}
