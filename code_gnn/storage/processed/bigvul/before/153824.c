void GLES2Implementation::SwapWithBounds(
    const std::vector<gfx::Rect>& rects,
    uint32_t flags,
    SwapCompletedCallback swap_completed,
    PresentationCallback presentation_callback) {
  std::vector<int> rects_data(rects.size() * 4);
  for (size_t i = 0; i < rects.size(); ++i) {
    rects_data[i * 4 + 0] = rects[i].x();
    rects_data[i * 4 + 1] = rects[i].y();
    rects_data[i * 4 + 2] = rects[i].width();
    rects_data[i * 4 + 3] = rects[i].height();
  }
  SwapBuffersWithBoundsCHROMIUM(
      PrepareNextSwapId(std::move(swap_completed),
                        std::move(presentation_callback)),
      rects.size(), rects_data.data(), flags);
}
