void RenderFrame::ForEach(RenderFrameVisitor* visitor) {
  FrameMap* frames = g_frame_map.Pointer();
  for (auto it = frames->begin(); it != frames->end(); ++it) {
    if (!visitor->Visit(it->second))
      return;
  }
}