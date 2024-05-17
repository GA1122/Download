static inline bool isInlineWithContinuation(LayoutObject* object) {
  if (!object->isBoxModelObject())
    return false;

  LayoutBoxModelObject* layoutObject = toLayoutBoxModelObject(object);
  if (!layoutObject->isLayoutInline())
    return false;

  return toLayoutInline(layoutObject)->continuation();
}
