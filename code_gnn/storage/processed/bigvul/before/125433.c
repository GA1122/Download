void GDataFileSystem::ReadDirectoryByPathAsyncOnUIThread(
    const FilePath& file_path,
    const ReadDirectoryWithSettingCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  FindEntryByPathAsyncOnUIThread(
      file_path,
      base::Bind(&GDataFileSystem::OnReadDirectory,
                 ui_weak_ptr_,
                 callback));
}
