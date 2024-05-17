ImageBitmap::ImageBitmap(ImageBitmap* bitmap,
                         Optional<IntRect> cropRect,
                         const ImageBitmapOptions& options) {
  RefPtr<Image> input = bitmap->bitmapImage();
  if (!input)
    return;
  ParsedOptions parsedOptions = parseOptions(options, cropRect, input->size());
  if (dstBufferSizeHasOverflow(parsedOptions))
    return;

  m_image = cropImage(input.get(), parsedOptions, bitmap->isPremultiplied()
                                                      ? PremultiplyAlpha
                                                      : DontPremultiplyAlpha);
  if (!m_image)
    return;
  m_image->setOriginClean(bitmap->originClean());
  m_image->setPremultiplied(parsedOptions.premultiplyAlpha);
}
