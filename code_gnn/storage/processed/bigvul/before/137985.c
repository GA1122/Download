static inline LayoutInline* startOfContinuations(LayoutObject* r) {
  if (r->isInlineElementContinuation()) {
    return toLayoutInline(r->node()->layoutObject());
  }

  if (r->isLayoutBlockFlow() &&
      toLayoutBlockFlow(r)->inlineElementContinuation())
    return toLayoutInline(toLayoutBlockFlow(r)
                              ->inlineElementContinuation()
                              ->node()
                              ->layoutObject());

  return 0;
}
