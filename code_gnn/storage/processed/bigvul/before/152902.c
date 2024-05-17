ParsedOptions parseOptions(const ImageBitmapOptions& options,
                           Optional<IntRect> cropRect,
                           IntSize sourceSize) {
  ParsedOptions parsedOptions;
  if (options.imageOrientation() == imageOrientationFlipY) {
    parsedOptions.flipY = true;
  } else {
    parsedOptions.flipY = false;
    DCHECK(options.imageOrientation() == imageBitmapOptionNone);
  }
  if (options.premultiplyAlpha() == imageBitmapOptionNone) {
    parsedOptions.premultiplyAlpha = false;
  } else {
    parsedOptions.premultiplyAlpha = true;
    DCHECK(options.premultiplyAlpha() == "default" ||
           options.premultiplyAlpha() == "premultiply");
  }

  int sourceWidth = sourceSize.width();
  int sourceHeight = sourceSize.height();
  if (!cropRect) {
    parsedOptions.cropRect = IntRect(0, 0, sourceWidth, sourceHeight);
  } else {
    parsedOptions.cropRect = normalizeRect(*cropRect);
  }
  if (!options.hasResizeWidth() && !options.hasResizeHeight()) {
    parsedOptions.resizeWidth = parsedOptions.cropRect.width();
    parsedOptions.resizeHeight = parsedOptions.cropRect.height();
  } else if (options.hasResizeWidth() && options.hasResizeHeight()) {
    parsedOptions.resizeWidth = options.resizeWidth();
    parsedOptions.resizeHeight = options.resizeHeight();
  } else if (options.hasResizeWidth() && !options.hasResizeHeight()) {
    parsedOptions.resizeWidth = options.resizeWidth();
    parsedOptions.resizeHeight =
        ceil(static_cast<float>(options.resizeWidth()) /
             parsedOptions.cropRect.width() * parsedOptions.cropRect.height());
  } else {
    parsedOptions.resizeHeight = options.resizeHeight();
    parsedOptions.resizeWidth =
        ceil(static_cast<float>(options.resizeHeight()) /
             parsedOptions.cropRect.height() * parsedOptions.cropRect.width());
  }
  if (static_cast<int>(parsedOptions.resizeWidth) ==
          parsedOptions.cropRect.width() &&
      static_cast<int>(parsedOptions.resizeHeight) ==
          parsedOptions.cropRect.height()) {
    parsedOptions.shouldScaleInput = false;
    return parsedOptions;
  }
  parsedOptions.shouldScaleInput = true;

  if (options.resizeQuality() == "high")
    parsedOptions.resizeQuality = kHigh_SkFilterQuality;
  else if (options.resizeQuality() == "medium")
    parsedOptions.resizeQuality = kMedium_SkFilterQuality;
  else if (options.resizeQuality() == "pixelated")
    parsedOptions.resizeQuality = kNone_SkFilterQuality;
  else
    parsedOptions.resizeQuality = kLow_SkFilterQuality;
  return parsedOptions;
 }
