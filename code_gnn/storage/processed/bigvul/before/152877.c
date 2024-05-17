ImageBitmap::ImageBitmap(HTMLCanvasElement* canvas,
                         Optional<IntRect> cropRect,
                         const ImageBitmapOptions& options) {
  ASSERT(canvas->isPaintable());
  RefPtr<Image> input = canvas->copiedImage(BackBuffer, PreferAcceleration);
  ParsedOptions parsedOptions =
      parseOptions(options, cropRect, canvas->bitmapSourceSize());
  if (dstBufferSizeHasOverflow(parsedOptions))
    return;

  bool isPremultiplyAlphaReverted = false;
  if (!parsedOptions.premultiplyAlpha) {
    parsedOptions.premultiplyAlpha = true;
    isPremultiplyAlphaReverted = true;
  }
  m_image = cropImage(input.get(), parsedOptions);
  if (!m_image)
    return;
  if (isPremultiplyAlphaReverted) {
    parsedOptions.premultiplyAlpha = false;
    m_image = StaticBitmapImage::create(
        premulSkImageToUnPremul(m_image->imageForCurrentFrame().get()));
  }
  if (!m_image)
    return;
  m_image->setOriginClean(canvas->originClean());
  m_image->setPremultiplied(parsedOptions.premultiplyAlpha);
}
