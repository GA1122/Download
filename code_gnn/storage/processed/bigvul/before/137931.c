FrameView* AXLayoutObject::documentFrameView() const {
  if (!getLayoutObject())
    return nullptr;

  return getLayoutObject()->document().view();
}
