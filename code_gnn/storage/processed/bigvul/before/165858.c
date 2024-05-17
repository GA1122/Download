static inline ImageBitmapSource* ToImageBitmapSourceInternal(
    const ImageBitmapSourceUnion& value,
    const ImageBitmapOptions* options,
    bool has_crop_rect) {
  if (value.IsHTMLVideoElement()) {
    UMA_HISTOGRAM_ENUMERATION("Blink.Canvas.CreateImageBitmapSource",
                              kCreateImageBitmapSourceHTMLVideoElement);
    return value.GetAsHTMLVideoElement();
  }
  if (value.IsHTMLImageElement()) {
    UMA_HISTOGRAM_ENUMERATION("Blink.Canvas.CreateImageBitmapSource",
                              kCreateImageBitmapSourceHTMLImageElement);
    return value.GetAsHTMLImageElement();
  }
  if (value.IsSVGImageElement()) {
    UMA_HISTOGRAM_ENUMERATION("Blink.Canvas.CreateImageBitmapSource",
                              kCreateImageBitmapSourceSVGImageElement);
    return value.GetAsSVGImageElement();
  }
  if (value.IsHTMLCanvasElement()) {
    UMA_HISTOGRAM_ENUMERATION("Blink.Canvas.CreateImageBitmapSource",
                              kCreateImageBitmapSourceHTMLCanvasElement);
    return value.GetAsHTMLCanvasElement();
  }
  if (value.IsBlob()) {
    UMA_HISTOGRAM_ENUMERATION("Blink.Canvas.CreateImageBitmapSource",
                              kCreateImageBitmapSourceBlob);
    return value.GetAsBlob();
  }
  if (value.IsImageData()) {
    UMA_HISTOGRAM_ENUMERATION("Blink.Canvas.CreateImageBitmapSource",
                              kCreateImageBitmapSourceImageData);
    return value.GetAsImageData();
  }
  if (value.IsImageBitmap()) {
    UMA_HISTOGRAM_ENUMERATION("Blink.Canvas.CreateImageBitmapSource",
                              kCreateImageBitmapSourceImageBitmap);
    return value.GetAsImageBitmap();
  }
  if (value.IsOffscreenCanvas()) {
    UMA_HISTOGRAM_ENUMERATION("Blink.Canvas.CreateImageBitmapSource",
                              kCreateImageBitmapSourceOffscreenCanvas);
    return value.GetAsOffscreenCanvas();
  }
  NOTREACHED();
  return nullptr;
}
