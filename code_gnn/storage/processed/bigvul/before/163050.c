static void FillFrameWithPredictableValues(const VideoFrame& frame) {
  for (size_t i = 0; i < VideoFrame::NumPlanes(frame.format()); ++i) {
    const gfx::Size& size =
        VideoFrame::PlaneSize(frame.format(), i, frame.coded_size());
    uint8_t* plane_ptr = const_cast<uint8_t*>(frame.data(i));
    for (int h = 0; h < size.height(); ++h) {
      const int row_index = h * frame.stride(i);
      for (int w = 0; w < size.width(); ++w) {
        const int index = row_index + w;
        plane_ptr[index] = static_cast<uint8_t>(w ^ h);
      }
    }
  }
}
