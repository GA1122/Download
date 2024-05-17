int GetBookmarkDropOperation(Profile* profile,
                             const ui::DropTargetEvent& event,
                             const bookmarks::BookmarkNodeData& data,
                             const BookmarkNode* parent,
                             int index) {
  const base::FilePath& profile_path = profile->GetPath();

  if (data.IsFromProfilePath(profile_path) && data.size() > 1)
    return ui::DragDropTypes::DRAG_NONE;

  if (!IsValidBookmarkDropLocation(profile, data, parent, index))
    return ui::DragDropTypes::DRAG_NONE;

  BookmarkModel* model = BookmarkModelFactory::GetForBrowserContext(profile);
  if (!model->client()->CanBeEditedByUser(parent))
    return ui::DragDropTypes::DRAG_NONE;

  const BookmarkNode* dragged_node =
      data.GetFirstNode(model, profile->GetPath());
  if (dragged_node) {
    if (!model->client()->CanBeEditedByUser(dragged_node)) {
      return ui::DragDropTypes::DRAG_COPY;
    }
    return ui::DragDropTypes::DRAG_MOVE;
  }

  return GetPreferredBookmarkDropOperation(event.source_operations(),
      ui::DragDropTypes::DRAG_COPY | ui::DragDropTypes::DRAG_LINK);
}
