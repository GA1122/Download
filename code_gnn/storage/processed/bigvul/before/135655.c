void FrameSelection::NotifyAccessibilityForSelectionChange() {
  if (GetSelectionInDOMTree().IsNone())
    return;
  AXObjectCache* cache = GetDocument().ExistingAXObjectCache();
  if (!cache)
    return;
  const Position& start = GetSelectionInDOMTree().ComputeStartPosition();
  cache->SelectionChanged(start.ComputeContainerNode());
}
