ImageBitmap::ImageBitmap(HTMLImageElement* image,
                         Optional<IntRect> cropRect,
                         Document* document,
                         const ImageBitmapOptions& options) {
  RefPtr<Image> input = image->cachedImage()->getImage();
  ParsedOptions parsedOptions =
      parseOptions(options, cropRect, image->bitmapSourceSize());
  if (dstBufferSizeHasOverflow(parsedOptions))
    return;

  if (options.colorSpaceConversion() == "none") {
    m_image = cropImage(input.get(), parsedOptions, PremultiplyAlpha,
                        ImageDecoder::ColorSpaceIgnored);
  } else {
    m_image = cropImage(input.get(), parsedOptions, PremultiplyAlpha,
                        ImageDecoder::ColorSpaceApplied);
  }
  if (!m_image)
    return;
  sk_sp<SkImage> skImage = m_image->imageForCurrentFrame();
  SkPixmap pixmap;
  if (!skImage->isTextureBacked() && !skImage->peekPixels(&pixmap)) {
    sk_sp<SkSurface> surface =
        SkSurface::MakeRasterN32Premul(skImage->width(), skImage->height());
    surface->getCanvas()->drawImage(skImage, 0, 0);
    m_image = StaticBitmapImage::create(surface->makeImageSnapshot());
  }
  if (!m_image)
    return;
  m_image->setOriginClean(
      !image->wouldTaintOrigin(document->getSecurityOrigin()));
  m_image->setPremultiplied(parsedOptions.premultiplyAlpha);
}
