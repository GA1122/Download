WebLocalFrameImpl* WebLocalFrameImpl::FromFrame(LocalFrame* frame) {
  if (!frame)
    return nullptr;
  return FromFrame(*frame);
}
