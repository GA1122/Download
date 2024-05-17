void GDataFileSystem::OnRemovedDocument(
    const FileOperationCallback& callback,
    const FilePath& file_path,
    GDataErrorCode status,
    const GURL& document_url) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  GDataFileError error = util::GDataToGDataFileError(status);

  if (error == GDATA_FILE_OK)
    error = RemoveEntryFromFileSystem(file_path);

  if (!callback.is_null()) {
    callback.Run(error);
  }
}
