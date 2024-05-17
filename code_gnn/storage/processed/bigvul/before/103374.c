void CapturerMac::CgBlit(const VideoFrameBuffer& buffer,
                         const InvalidRects& rects) {
  if (last_buffer_)
    memcpy(buffer.ptr(), last_buffer_,
           buffer.bytes_per_row() * buffer.size().height());
  last_buffer_ = buffer.ptr();
  CGDirectDisplayID main_display = CGMainDisplayID();
  uint8* display_base_address =
      reinterpret_cast<uint8*>(CGDisplayBaseAddress(main_display));
  int src_bytes_per_row = CGDisplayBytesPerRow(main_display);
  int src_bytes_per_pixel = CGDisplayBitsPerPixel(main_display) / 8;
  for (InvalidRects::iterator i = rects.begin(); i != rects.end(); ++i) {
    int src_row_offset =  i->x() * src_bytes_per_pixel;
    int dst_row_offset = i->x() * sizeof(uint32_t);
    int rect_width_in_bytes = i->width() * src_bytes_per_pixel;
    int ymax = i->height() + i->y();
    for (int y = i->y(); y < ymax; ++y) {
      memcpy(buffer.ptr() + y * buffer.bytes_per_row() + dst_row_offset,
             display_base_address + y * src_bytes_per_row + src_row_offset,
             rect_width_in_bytes);
    }
  }
}
