void FileAPIMessageFilter::DidOpenFileSystem(int request_id,
                                             base::PlatformFileError result,
                                             const std::string& name,
                                             const GURL& root) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  if (result == base::PLATFORM_FILE_OK) {
    DCHECK(root.is_valid());
    Send(new FileSystemMsg_DidOpenFileSystem(request_id, name, root));
  } else {
    Send(new FileSystemMsg_DidFail(request_id, result));
  }
}
