ImageData* WebGLRenderingContextBase::ToImageData(SnapshotReason reason) {
  ImageData* image_data = nullptr;
  if (GetDrawingBuffer()) {
    image_data = PaintRenderingResultsToImageData(kBackBuffer);
    if (image_data) {
      return image_data;
    }

    int width = GetDrawingBuffer()->Size().Width();
    int height = GetDrawingBuffer()->Size().Height();
    SkImageInfo image_info =
        SkImageInfo::Make(width, height, kRGBA_8888_SkColorType,
                          CreationAttributes().alpha() ? kPremul_SkAlphaType
                                                       : kOpaque_SkAlphaType);
    sk_sp<SkImage> snapshot = MakeImageSnapshot(image_info);
    if (snapshot) {
      image_data = ImageData::Create(GetDrawingBuffer()->Size());
      snapshot->readPixels(image_info, image_data->data()->Data(),
                           image_info.minRowBytes(), 0, 0);
    }
  }
  return image_data;
}
