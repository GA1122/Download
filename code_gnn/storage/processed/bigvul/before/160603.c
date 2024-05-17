void RenderFrame::ForEach(RenderFrameVisitor* visitor) {
  FrameMap* frames = g_frame_map.Pointer();
  for (FrameMap::iterator it = frames->begin(); it != frames->end(); ++it) {
    if (!visitor->Visit(it->second))
      return;
  }
}
