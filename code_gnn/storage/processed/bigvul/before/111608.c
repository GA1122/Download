  bool UpdateContent(const std::vector<DocumentFeed*>& list,
                     int largest_changestamp) {
    GURL unused;
    return file_system_->UpdateFromFeed(
        list,
        FROM_SERVER,
        largest_changestamp,
        root_feed_changestamp_++) == base::PLATFORM_FILE_OK;
  }
