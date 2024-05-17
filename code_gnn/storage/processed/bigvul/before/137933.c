static inline LayoutObject* endOfContinuations(LayoutObject* layoutObject) {
  LayoutObject* prev = layoutObject;
  LayoutObject* cur = layoutObject;

  if (!cur->isLayoutInline() && !cur->isLayoutBlockFlow())
    return layoutObject;

  while (cur) {
    prev = cur;
    if (cur->isLayoutInline()) {
      cur = toLayoutInline(cur)->inlineElementContinuation();
      ASSERT(cur || !toLayoutInline(prev)->continuation());
    } else {
      cur = toLayoutBlockFlow(cur)->inlineElementContinuation();
    }
  }

  return prev;
}
