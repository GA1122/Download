void OnCacheUpdatedForAddUploadedFile(
    const base::Closure& callback,
    GDataFileError  ,
    const std::string&  ,
    const std::string&  ) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  if (!callback.is_null())
    callback.Run();
}
