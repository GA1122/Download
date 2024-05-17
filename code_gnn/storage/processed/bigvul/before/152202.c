void RenderFrameImpl::BindToFrame(blink::WebNavigationControl* frame) {
  DCHECK(!frame_);

  std::pair<FrameMap::iterator, bool> result =
      g_frame_map.Get().emplace(frame, this);
  CHECK(result.second) << "Inserting a duplicate item.";

  frame_ = frame;
}
