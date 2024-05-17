size_t V4L2JpegEncodeAccelerator::GetMaxCodedBufferSize(
    const gfx::Size& picture_size) {
  return picture_size.GetArea() * 3 / 2 + kJpegDefaultHeaderSize;
}
