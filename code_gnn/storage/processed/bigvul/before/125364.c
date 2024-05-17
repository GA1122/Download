void GDataFileSystem::GetEntryInfoByPathAsyncOnUIThread(
    const FilePath& file_path,
    const GetEntryInfoCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  FindEntryByPathAsyncOnUIThread(
      file_path,
      base::Bind(&GDataFileSystem::OnGetEntryInfo,
                 ui_weak_ptr_,
                 callback));
}
