LocalFrame* GetLocalParentFrame(WebLocalFrameImpl* frame) {
  WebFrame* parent = frame->Parent();
  if (!parent || !parent->IsWebLocalFrame())
    return nullptr;

  return ToWebLocalFrameImpl(parent)->GetFrame();
}
