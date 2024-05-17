FileSystemDelegate* ChromeExtensionsAPIClient::GetFileSystemDelegate() {
  if (!file_system_delegate_)
    file_system_delegate_ = base::MakeUnique<ChromeFileSystemDelegate>();
  return file_system_delegate_.get();
}
