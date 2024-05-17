void RunGetResourceIdsOfBacklogCallback(
    const GetResourceIdsOfBacklogCallback& callback,
    std::vector<std::string>* to_fetch,
    std::vector<std::string>* to_upload) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(to_fetch);
  DCHECK(to_upload);

  if (!callback.is_null())
    callback.Run(*to_fetch, *to_upload);
}
