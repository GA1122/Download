bool BookmarksMoveFunction::RunImpl() {
  if (!EditBookmarksEnabled())
    return false;

  scoped_ptr<bookmarks::Move::Params> params(
      bookmarks::Move::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  int64 id;
  if (!base::StringToInt64(params->id, &id)) {
    error_ = keys::kInvalidIdError;
    return false;
  }

  BookmarkModel* model = BookmarkModelFactory::GetForProfile(profile());
  const BookmarkNode* node = model->GetNodeByID(id);
  if (!node) {
    error_ = keys::kNoNodeError;
    return false;
  }
  if (model->is_permanent_node(node)) {
    error_ = keys::kModifySpecialError;
    return false;
  }

  const BookmarkNode* parent = NULL;
  if (!params->destination.parent_id.get()) {
    parent = node->parent();
  } else {
    int64 parentId;
    if (!GetBookmarkIdAsInt64(*params->destination.parent_id, &parentId))
      return false;

    parent = model->GetNodeByID(parentId);
  }
  if (!parent) {
    error_ = keys::kNoParentError;
    return false;
  }
  if (parent == model->root_node()) {
    error_ = keys::kModifySpecialError;
    return false;
  }

  int index;
  if (params->destination.index.get()) {   
    index = *params->destination.index;
    if (index > parent->child_count() || index < 0) {
      error_ = keys::kInvalidIndexError;
      return false;
    }
  } else {
    index = parent->child_count();
  }

  model->Move(node, parent, index);

  scoped_ptr<BookmarkTreeNode> tree_node(
      bookmark_api_helpers::GetBookmarkTreeNode(node, false, false));
  results_ = bookmarks::Move::Results::Create(*tree_node);

  return true;
}
