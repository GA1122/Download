void AXObjectCacheImpl::setCanvasObjectBounds(HTMLCanvasElement* canvas,
                                              Element* element,
                                              const LayoutRect& rect) {
  AXObject* obj = getOrCreate(element);
  if (!obj)
    return;

  AXObject* axCanvas = getOrCreate(canvas);
  if (!axCanvas)
    return;

  obj->setElementRect(rect, axCanvas);
}
