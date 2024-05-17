 bool ChangeFilePermissions(const FilePath& file_path, mode_t permissions) {
   if (HANDLE_EINTR(chmod(file_path.value().c_str(), permissions)) != 0) {
    PLOG(ERROR) << "Error changing permissions of " << file_path.value();
    return false;
  }
  DVLOG(1) << "Changed permissions of " << file_path.value();
   return true;
 }
