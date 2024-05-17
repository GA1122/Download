Element* Document::ElementFromPoint(int x, int y) const {
  if (GetLayoutViewItem().IsNull())
    return 0;

  return TreeScope::ElementFromPoint(x, y);
}
