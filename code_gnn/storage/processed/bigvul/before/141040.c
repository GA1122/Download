Element* Document::ElementFromPoint(double x, double y) const {
  if (!GetLayoutView())
    return nullptr;

  return TreeScope::ElementFromPoint(x, y);
}
