void FileAPIMessageFilter::DidCreateSnapshot(
    int request_id,
    const base::Callback<void(const FilePath&)>& register_file_callback,
    base::PlatformFileError result,
    const base::PlatformFileInfo& info,
    const FilePath& platform_path,
    const scoped_refptr<webkit_blob::ShareableFileReference>& unused) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  if (result != base::PLATFORM_FILE_OK) {
    Send(new FileSystemMsg_DidFail(request_id, result));
    return;
  }

  register_file_callback.Run(platform_path);

  Send(new FileSystemMsg_DidReadMetadata(request_id, info, platform_path));
 }
