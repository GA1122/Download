long TestWebKitPlatformSupport::databaseGetFileAttributes(
    const WebKit::WebString& vfs_file_name) {
  return SimpleDatabaseSystem::GetInstance()->GetFileAttributes(
      vfs_file_name);
}
