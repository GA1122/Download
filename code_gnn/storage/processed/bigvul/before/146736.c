HeapVector<Member<Element>> Document::ElementsFromPoint(int x, int y) const {
  if (GetLayoutViewItem().IsNull())
    return HeapVector<Member<Element>>();
  return TreeScope::ElementsFromPoint(x, y);
}
