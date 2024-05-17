String AXLayoutObject::imageDataUrl(const IntSize& maxSize) const {
  Node* node = getNode();
  if (!node)
    return String();

  ImageBitmapOptions options;
  ImageBitmap* imageBitmap = nullptr;
  Document* document = &node->document();
  if (isHTMLImageElement(node)) {
    imageBitmap = ImageBitmap::create(toHTMLImageElement(node),
                                      Optional<IntRect>(), document, options);
  } else if (isHTMLCanvasElement(node)) {
    imageBitmap = ImageBitmap::create(toHTMLCanvasElement(node),
                                      Optional<IntRect>(), options);
  } else if (isHTMLVideoElement(node)) {
    imageBitmap = ImageBitmap::create(toHTMLVideoElement(node),
                                      Optional<IntRect>(), document, options);
  }
  if (!imageBitmap)
    return String();

  StaticBitmapImage* bitmapImage = imageBitmap->bitmapImage();
  if (!bitmapImage)
    return String();

  sk_sp<SkImage> image = bitmapImage->imageForCurrentFrame();
  if (!image || image->width() <= 0 || image->height() <= 0)
    return String();

  float xScale = maxSize.width() ? maxSize.width() * 1.0 / image->width() : 1.0;
  float yScale =
      maxSize.height() ? maxSize.height() * 1.0 / image->height() : 1.0;
  float scale = std::min(xScale, yScale);
  if (scale >= 1.0)
    scale = 1.0;
  int width = std::round(image->width() * scale);
  int height = std::round(image->height() * scale);

  SkBitmap bitmap;
  bitmap.allocPixels(SkImageInfo::MakeN32(width, height, kPremul_SkAlphaType));
  SkCanvas canvas(bitmap);
  canvas.clear(SK_ColorTRANSPARENT);
  canvas.drawImageRect(image, SkRect::MakeIWH(width, height), nullptr);

  SkImageInfo info = SkImageInfo::Make(width, height, kRGBA_8888_SkColorType,
                                       kUnpremul_SkAlphaType);
  size_t rowBytes = info.minRowBytes();
  Vector<char> pixelStorage(info.getSafeSize(rowBytes));
  SkPixmap pixmap(info, pixelStorage.data(), rowBytes);
  if (!SkImage::MakeFromBitmap(bitmap)->readPixels(pixmap, 0, 0))
    return String();

  String dataUrl =
      ImageDataBuffer(
          IntSize(width, height),
          reinterpret_cast<const unsigned char*>(pixelStorage.data()))
          .toDataURL("image/png", 1.0);
  return dataUrl;
}
