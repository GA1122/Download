inline float ParentTextZoomFactor(LocalFrame* frame) {
  Frame* parent = frame->Tree().Parent();
  if (!parent || !parent->IsLocalFrame())
    return 1;
  return ToLocalFrame(parent)->TextZoomFactor();
}