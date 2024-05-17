skia::PlatformCanvas* RenderProcessImpl::GetDrawingCanvas(
    TransportDIB** memory, const gfx::Rect& rect) {
  int width = rect.width();
  int height = rect.height();
  const size_t stride = skia::PlatformCanvas::StrideForWidth(rect.width());
#if defined(OS_LINUX) || defined(OS_OPENBSD)
  const size_t max_size = base::SysInfo::MaxSharedMemorySize();
#else
  const size_t max_size = 0;
#endif

  if ((max_size != 0) && (height * stride > max_size))
    height = max_size / stride;

  const size_t size = height * stride;

  if (!GetTransportDIBFromCache(memory, size)) {
    *memory = CreateTransportDIB(size);
    if (!*memory)
      return NULL;
  }

  return (*memory)->GetPlatformCanvas(width, height);
}
