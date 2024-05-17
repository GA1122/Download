void EncodeBookmarkURLAndFavicon(sqlite_utils::SQLStatement* old_value_query,
                                 int old_value_column,
                                 sync_pb::EntitySpecifics* mutable_new_value) {
  bool old_is_bookmark_object = old_value_query->column_bool(old_value_column);
  std::string old_url = old_value_query->column_string(old_value_column + 1);
  std::string old_favicon;
  old_value_query->column_blob_as_string(old_value_column + 2, &old_favicon);
  bool old_is_dir = old_value_query->column_bool(old_value_column + 3);

  if (old_is_bookmark_object) {
    sync_pb::BookmarkSpecifics* bookmark_data =
        mutable_new_value->MutableExtension(sync_pb::bookmark);
    if (!old_is_dir) {
      bookmark_data->set_url(old_url);
      bookmark_data->set_favicon(old_favicon);
    }
  }
}
