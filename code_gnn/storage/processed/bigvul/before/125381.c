void GDataFileSystem::LoadRootFeedFromCacheForTesting() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  feed_loader_->LoadFromCache(
      false,   
      FilePath(),
      FindEntryCallback());
}
