static inline LayoutObject* firstChildInContinuation(
    const LayoutInline& layoutObject) {
  LayoutBoxModelObject* r = layoutObject.continuation();

  while (r) {
    if (r->isLayoutBlock())
      return r;
    if (LayoutObject* child = r->slowFirstChild())
      return child;
    r = toLayoutInline(r)->continuation();
  }

  return 0;
}
