FrameView* AXObject::documentFrameView() const {
  const AXObject* object = this;
  while (object && !object->isAXLayoutObject())
    object = object->parentObject();

  if (!object)
    return 0;

  return object->documentFrameView();
}
