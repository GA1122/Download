static LayoutBoxModelObject* nextContinuation(LayoutObject* layoutObject) {
  ASSERT(layoutObject);
  if (layoutObject->isLayoutInline() && !layoutObject->isAtomicInlineLevel())
    return toLayoutInline(layoutObject)->continuation();
  if (layoutObject->isLayoutBlockFlow())
    return toLayoutBlockFlow(layoutObject)->inlineElementContinuation();
  return 0;
}
