skia::PlatformCanvas* TransportDIB::GetPlatformCanvas(int w, int h) {
  if (address_ == kInvalidAddress && !Map())
    return NULL;
  scoped_ptr<skia::PlatformCanvas> canvas(new skia::PlatformCanvas);
  if (!canvas->initialize(w, h, true, reinterpret_cast<uint8_t*>(memory())))
    return NULL;
  return canvas.release();
}
