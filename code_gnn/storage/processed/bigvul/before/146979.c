WebFrame* WebLocalFrameImpl::FindFrameByName(const WebString& name) {
  Frame* result = GetFrame()->Tree().Find(name);
  return WebFrame::FromFrame(result);
}
