static inline bool lastChildHasContinuation(LayoutObject* layoutObject) {
  LayoutObject* lastChild = layoutObject->slowLastChild();
  return lastChild && isInlineWithContinuation(lastChild);
}
