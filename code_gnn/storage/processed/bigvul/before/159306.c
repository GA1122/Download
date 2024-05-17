WebGLRenderingContextBase::GetStaticBitmapImage() {
  if (!GetDrawingBuffer())
    return nullptr;

  if (CreationAttributes().preserveDrawingBuffer()) {
    int width = GetDrawingBuffer()->Size().Width();
    int height = GetDrawingBuffer()->Size().Height();
    SkImageInfo image_info = SkImageInfo::Make(
        width, height, kRGBA_8888_SkColorType, ColorParams().GetSkAlphaType());
    return MakeImageSnapshot(image_info);
  }
  return GetDrawingBuffer()->TransferToStaticBitmapImage();
}
