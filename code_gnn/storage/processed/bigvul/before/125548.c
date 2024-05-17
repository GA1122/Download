GDataFileError GDataWapiFeedProcessor::ApplyFeeds(
    const std::vector<DocumentFeed*>& feed_list,
    int64 start_changestamp,
    int64 root_feed_changestamp,
    std::set<FilePath>* changed_dirs) {
  bool is_delta_feed = start_changestamp != 0;

  directory_service_->set_origin(FROM_SERVER);

  int64 delta_feed_changestamp = 0;
  FeedToFileResourceMapUmaStats uma_stats;
  FileResourceIdMap file_map;
  GDataFileError error = FeedToFileResourceMap(feed_list,
                                               &file_map,
                                               &delta_feed_changestamp,
                                               &uma_stats);
  if (error != GDATA_FILE_OK)
    return error;

  ApplyFeedFromFileUrlMap(
      is_delta_feed,
      is_delta_feed ? delta_feed_changestamp : root_feed_changestamp,
      &file_map,
      changed_dirs);

  if (!is_delta_feed)
    UpdateFileCountUmaHistograms(uma_stats);

  return GDATA_FILE_OK;
}
