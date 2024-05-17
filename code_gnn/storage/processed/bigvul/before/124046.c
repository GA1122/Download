bool BookmarksGetTreeFunction::RunImpl() {
  scoped_ptr<bookmarks::Get::Params> params(
      bookmarks::Get::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  std::vector<linked_ptr<BookmarkTreeNode> > nodes;
  BookmarkModel* model = BookmarkModelFactory::GetForProfile(profile());
  if (params->id_or_id_list.as_array) {
    std::vector<std::string>* ids = params->id_or_id_list.as_array.get();
    size_t count = ids->size();
    EXTENSION_FUNCTION_VALIDATE(count > 0);
    for (size_t i = 0; i < count; ++i) {
      int64 id;
      if (!GetBookmarkIdAsInt64(ids->at(i), &id))
        return false;
      const BookmarkNode* node = model->GetNodeByID(id);
      if (!node) {
        error_ = keys::kNoNodeError;
        return false;
      } else {
        bookmark_api_helpers::AddNode(node, &nodes, false);
      }
    }
  } else {
    int64 id;
    if (!GetBookmarkIdAsInt64(*params->id_or_id_list.as_string, &id))
      return false;
    const BookmarkNode* node = model->GetNodeByID(id);
    if (!node) {
      error_ = keys::kNoNodeError;
      return false;
    }
    bookmark_api_helpers::AddNode(node, &nodes, false);
  }

  results_ = bookmarks::Get::Results::Create(nodes);
  return true;
}
