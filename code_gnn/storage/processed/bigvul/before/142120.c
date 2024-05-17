bool IsValidBookmarkDropLocation(Profile* profile,
                                 const bookmarks::BookmarkNodeData& data,
                                 const BookmarkNode* drop_parent,
                                 int index) {
  if (!drop_parent->is_folder()) {
    NOTREACHED();
    return false;
  }

  if (!data.is_valid())
    return false;

  BookmarkModel* model = BookmarkModelFactory::GetForBrowserContext(profile);
  if (!model->client()->CanBeEditedByUser(drop_parent))
    return false;

  const base::FilePath& profile_path = profile->GetPath();
  if (data.IsFromProfilePath(profile_path)) {
    std::vector<const BookmarkNode*> nodes = data.GetNodes(model, profile_path);
    for (size_t i = 0; i < nodes.size(); ++i) {
      const BookmarkNode* node = nodes[i];
      int node_index = (drop_parent == node->parent()) ?
          drop_parent->GetIndexOf(nodes[i]) : -1;
      if (node_index != -1 && (index == node_index || index == node_index + 1))
        return false;

      if (drop_parent->HasAncestor(node))
        return false;
    }
    return true;
  }
  return true;
}
