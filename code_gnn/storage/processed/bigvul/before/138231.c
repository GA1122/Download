AXObject* AXObjectCacheImpl::focusedImageMapUIElement(
    HTMLAreaElement* areaElement) {
  if (!areaElement)
    return 0;

  HTMLImageElement* imageElement = areaElement->imageElement();
  if (!imageElement)
    return 0;

  AXObject* axLayoutImage = getOrCreate(imageElement);
  if (!axLayoutImage)
    return 0;

  const AXObject::AXObjectVector& imageChildren = axLayoutImage->children();
  unsigned count = imageChildren.size();
  for (unsigned k = 0; k < count; ++k) {
    AXObject* child = imageChildren[k];
    if (!child->isImageMapLink())
      continue;

    if (toAXImageMapLink(child)->areaElement() == areaElement)
      return child;
  }

  return 0;
}
