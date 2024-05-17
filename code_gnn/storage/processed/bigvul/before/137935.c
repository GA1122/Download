static inline LayoutObject* firstChildConsideringContinuation(
    LayoutObject* layoutObject) {
  LayoutObject* firstChild = layoutObject->slowFirstChild();

  if (firstChild && firstChild->isText() &&
      toLayoutText(firstChild)->isTextFragment() &&
      toLayoutTextFragment(firstChild)->firstLetterPseudoElement())
    return nullptr;

  if (!firstChild && isInlineWithContinuation(layoutObject))
    firstChild = firstChildInContinuation(toLayoutInline(*layoutObject));

  return firstChild;
}
