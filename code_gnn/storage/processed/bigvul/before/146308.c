PassRefPtr<Image> WebGLRenderingContextBase::GetImage(
    AccelerationHint hint,
    SnapshotReason reason) const {
  if (!GetDrawingBuffer())
    return nullptr;

  GetDrawingBuffer()->ResolveAndBindForReadAndDraw();
  IntSize size = ClampedCanvasSize();
  OpacityMode opacity_mode =
      CreationAttributes().hasAlpha() ? kNonOpaque : kOpaque;
  std::unique_ptr<AcceleratedImageBufferSurface> surface =
      WTF::MakeUnique<AcceleratedImageBufferSurface>(size, opacity_mode);
  if (!surface->IsValid())
    return nullptr;
  std::unique_ptr<ImageBuffer> buffer = ImageBuffer::Create(std::move(surface));
  if (!buffer->CopyRenderingResultsFromDrawingBuffer(GetDrawingBuffer(),
                                                     kBackBuffer)) {
    NOTREACHED();
    return nullptr;
  }
  return buffer->NewImageSnapshot(hint, reason);
}
