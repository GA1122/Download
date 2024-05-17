HeapVector<Member<Element>> Document::ElementsFromPoint(double x,
                                                        double y) const {
  if (GetLayoutViewItem().IsNull())
    return HeapVector<Member<Element>>();
  return TreeScope::ElementsFromPoint(x, y);
}
