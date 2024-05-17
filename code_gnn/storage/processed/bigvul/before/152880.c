ImageBitmap::ImageBitmap(PassRefPtr<StaticBitmapImage> image,
                         Optional<IntRect> cropRect,
                         const ImageBitmapOptions& options) {
  bool originClean = image->originClean();
  RefPtr<Image> input = image;
  ParsedOptions parsedOptions = parseOptions(options, cropRect, input->size());
  if (dstBufferSizeHasOverflow(parsedOptions))
    return;

  m_image = cropImage(input.get(), parsedOptions);
  if (!m_image)
    return;

  m_image->setOriginClean(originClean);
  m_image->setPremultiplied(parsedOptions.premultiplyAlpha);
}
