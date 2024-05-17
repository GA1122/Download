LocalFrameClientImpl* LocalFrameClientImpl::Create(WebLocalFrameImpl* frame) {
  return new LocalFrameClientImpl(frame);
}
