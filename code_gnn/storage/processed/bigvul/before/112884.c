void GDataCache::GetResourceIdsOfExistingPinnedFilesOnUIThread(
    const GetResourceIdsCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  std::vector<std::string>* resource_ids = new std::vector<std::string>;
  pool_->GetSequencedTaskRunner(sequence_token_)->PostTaskAndReply(
      FROM_HERE,
      base::Bind(&GDataCache::GetResourceIdsOfExistingPinnedFiles,
                 base::Unretained(this),
                 resource_ids),
      base::Bind(&RunGetResourceIdsCallback,
                 callback,
                 base::Owned(resource_ids)));
}
