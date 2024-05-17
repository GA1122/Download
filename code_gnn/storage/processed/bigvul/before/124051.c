bool BookmarksSearchFunction::RunImpl() {
  scoped_ptr<bookmarks::Search::Params> params(
      bookmarks::Search::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  PrefService* prefs = PrefServiceFromBrowserContext(profile_);
  std::string lang = prefs->GetString(prefs::kAcceptLanguages);
  std::vector<const BookmarkNode*> nodes;
  bookmark_utils::GetBookmarksContainingText(
      BookmarkModelFactory::GetForProfile(profile()),
      UTF8ToUTF16(params->query),
      std::numeric_limits<int>::max(),
      lang,
      &nodes);

  std::vector<linked_ptr<BookmarkTreeNode> > tree_nodes;
  for (std::vector<const BookmarkNode*>::iterator node_iter = nodes.begin();
       node_iter != nodes.end(); ++node_iter) {
    bookmark_api_helpers::AddNode(*node_iter, &tree_nodes, false);
  }

  results_ = bookmarks::Search::Results::Create(tree_nodes);
  return true;
}
