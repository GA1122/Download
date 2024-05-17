bool BookmarksGetSubTreeFunction::RunImpl() {
  std::vector<linked_ptr<BookmarkTreeNode> > nodes;
  const BookmarkNode* node =
      BookmarkModelFactory::GetForProfile(profile())->root_node();
  bookmark_api_helpers::AddNode(node, &nodes, true);
  results_ = bookmarks::GetTree::Results::Create(nodes);
  return true;
}
