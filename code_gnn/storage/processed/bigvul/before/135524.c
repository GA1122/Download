bool Editor::CanDeleteRange(const EphemeralRange& range) const {
  if (range.IsCollapsed())
    return false;

  Node* start_container = range.StartPosition().ComputeContainerNode();
  Node* end_container = range.EndPosition().ComputeContainerNode();
  if (!start_container || !end_container)
    return false;

  return HasEditableStyle(*start_container) && HasEditableStyle(*end_container);
}
