long long TestWebKitPlatformSupport::databaseGetFileSize(
    const WebKit::WebString& vfs_file_name) {
  return SimpleDatabaseSystem::GetInstance()->GetFileSize(vfs_file_name);
}
