skia::PlatformCanvas* MockRenderProcess::GetDrawingCanvas(
    TransportDIB** memory,
    const gfx::Rect& rect) {
  size_t stride = skia::PlatformCanvas::StrideForWidth(rect.width());
  size_t size = stride * rect.height();

  *memory = TransportDIB::Create(size, transport_dib_next_sequence_number_++);
  if (!*memory)
    return NULL;
  return (*memory)->GetPlatformCanvas(rect.width(), rect.height());
}
