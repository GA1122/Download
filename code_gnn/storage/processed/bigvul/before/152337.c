std::vector<int> RenderFrameImpl::UniqueNameFrameAdapter::GetFramePosition(
    BeginPoint begin_point) const {
  std::vector<int> result;
  blink::WebFrame* parent = begin_point == BeginPoint::kParentFrame
                                ? GetWebFrame()->Parent()
                                : GetWebFrame();
  blink::WebFrame* child =
      begin_point == BeginPoint::kParentFrame ? GetWebFrame() : nullptr;
  while (parent) {
    int position_in_parent = 0;
    blink::WebFrame* sibling = parent->FirstChild();
    while (sibling != child) {
      sibling = sibling->NextSibling();
      ++position_in_parent;
    }
    result.push_back(position_in_parent);

    child = parent;
    parent = parent->Parent();
  }
  return result;
}
