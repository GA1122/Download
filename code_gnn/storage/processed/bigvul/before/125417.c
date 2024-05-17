void GDataFileSystem::OnGetModifiedFileInfoCompleteForCloseFile(
    const FilePath& file_path,
    base::PlatformFileInfo* file_info,
    bool* get_file_info_result,
    const FileOperationCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (!*get_file_info_result) {
    if (!callback.is_null())
      callback.Run(GDATA_FILE_ERROR_NOT_FOUND);
    return;
  }

  FindEntryByPathAsyncOnUIThread(
      file_path,
      base::Bind(&GDataFileSystem::OnGetEntryCompleteForCloseFile,
                 ui_weak_ptr_,
                 file_path,
                 *file_info,
                 callback));
}
