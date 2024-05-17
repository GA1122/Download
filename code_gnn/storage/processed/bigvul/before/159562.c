Element* Document::ElementFromPoint(double x, double y) const {
  if (GetLayoutViewItem().IsNull())
    return nullptr;

  return TreeScope::ElementFromPoint(x, y);
}
