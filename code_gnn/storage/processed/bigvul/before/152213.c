RenderFrameImpl::UniqueNameFrameAdapter::CollectAncestorNames(
    BeginPoint begin_point,
    bool (*should_stop)(base::StringPiece)) const {
  std::vector<base::StringPiece> result;
  for (blink::WebFrame* frame = begin_point == BeginPoint::kParentFrame
                                    ? GetWebFrame()->Parent()
                                    : GetWebFrame();
       frame; frame = frame->Parent()) {
    result.push_back(UniqueNameForWebFrame(frame));
    if (should_stop(result.back()))
      break;
  }
  return result;
}
