bool BookmarksGetChildrenFunction::RunImpl() {
  scoped_ptr<bookmarks::GetChildren::Params> params(
      bookmarks::GetChildren::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  int64 id;
  if (!GetBookmarkIdAsInt64(params->id, &id))
    return false;

  std::vector<linked_ptr<BookmarkTreeNode> > nodes;
  const BookmarkNode* node =
      BookmarkModelFactory::GetForProfile(profile())->GetNodeByID(id);
  if (!node) {
    error_ = keys::kNoNodeError;
    return false;
  }
  int child_count = node->child_count();
  for (int i = 0; i < child_count; ++i) {
    const BookmarkNode* child = node->GetChild(i);
    bookmark_api_helpers::AddNode(child, &nodes, false);
  }

  results_ = bookmarks::GetChildren::Results::Create(nodes);
  return true;
}
