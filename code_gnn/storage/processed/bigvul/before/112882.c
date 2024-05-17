void GDataCache::GetResourceIdsOfBacklogOnUIThread(
    const GetResourceIdsOfBacklogCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  std::vector<std::string>* to_fetch = new std::vector<std::string>;
  std::vector<std::string>* to_upload = new std::vector<std::string>;
  pool_->GetSequencedTaskRunner(sequence_token_)->PostTaskAndReply(
      FROM_HERE,
      base::Bind(&GDataCache::GetResourceIdsOfBacklog,
                 base::Unretained(this),
                 to_fetch,
                 to_upload),
      base::Bind(&RunGetResourceIdsOfBacklogCallback,
                 callback,
                 base::Owned(to_fetch),
                 base::Owned(to_upload)));
}
