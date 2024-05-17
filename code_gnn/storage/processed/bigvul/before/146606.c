IntSize DrawingBuffer::AdjustSize(const IntSize& desired_size,
                                  const IntSize& cur_size,
                                  int max_texture_size) {
  IntSize adjusted_size = desired_size;

  if (adjusted_size.Height() > max_texture_size)
    adjusted_size.SetHeight(max_texture_size);

  if (adjusted_size.Width() > max_texture_size)
    adjusted_size.SetWidth(max_texture_size);

  return adjusted_size;
}
