int RenderFrameImpl::UniqueNameFrameAdapter::GetSiblingCount() const {
  int sibling_count = 0;
  for (blink::WebFrame* frame = GetWebFrame()->Parent()->FirstChild(); frame;
       frame = frame->NextSibling()) {
    if (frame == GetWebFrame())
      continue;
    ++sibling_count;
  }
  return sibling_count;
}
