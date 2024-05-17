int TestWebKitPlatformSupport::databaseDeleteFile(
    const WebKit::WebString& vfs_file_name, bool sync_dir) {
  return SimpleDatabaseSystem::GetInstance()->DeleteFile(
      vfs_file_name, sync_dir);
}
