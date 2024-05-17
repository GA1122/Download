GDataFileError GDataFileSystem::UpdateFromFeedForTesting(
    const std::vector<DocumentFeed*>& feed_list,
    int64 start_changestamp,
    int64 root_feed_changestamp) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  return feed_loader_->UpdateFromFeed(feed_list,
                                      start_changestamp,
                                      root_feed_changestamp);
}
