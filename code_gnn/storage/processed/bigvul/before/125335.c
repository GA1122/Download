void GDataFileSystem::AddUploadedFile(
    UploadMode upload_mode,
    const FilePath& virtual_dir_path,
    scoped_ptr<DocumentEntry> entry,
    const FilePath& file_content_path,
    GDataCache::FileOperationType cache_operation,
    const base::Closure& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  base::MessageLoopProxy::current()->PostTask(
      FROM_HERE,
      base::Bind(&GDataFileSystem::AddUploadedFileOnUIThread,
                 ui_weak_ptr_,
                 upload_mode,
                 virtual_dir_path,
                 base::Passed(&entry),
                 file_content_path,
                 cache_operation,
                 callback));
}
