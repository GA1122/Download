void OnAddUploadFileCompleted(
    const FileOperationCallback& callback,
    GDataFileError error) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  if (!callback.is_null())
    callback.Run(error);
}
