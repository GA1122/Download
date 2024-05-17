void GDataFileSystem::CreateFileOnUIThread(
    const FilePath& file_path,
    bool is_exclusive,
    const FileOperationCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  FindEntryByPathAsyncOnUIThread(
      file_path,
      base::Bind(&GDataFileSystem::OnGetEntryInfoForCreateFile,
                 ui_weak_ptr_,
                 file_path,
                 is_exclusive,
                 callback));
}
