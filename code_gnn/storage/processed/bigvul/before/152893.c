ScriptPromise ImageBitmap::createImageBitmap(ScriptState* scriptState,
                                             EventTarget& eventTarget,
                                             Optional<IntRect> cropRect,
                                             const ImageBitmapOptions& options,
                                             ExceptionState& exceptionState) {
  if ((cropRect &&
       !isSourceSizeValid(cropRect->width(), cropRect->height(),
                          exceptionState)) ||
      !isSourceSizeValid(width(), height(), exceptionState))
    return ScriptPromise();
  if (!isResizeOptionValid(options, exceptionState))
    return ScriptPromise();
  return ImageBitmapSource::fulfillImageBitmap(scriptState,
                                               create(this, cropRect, options));
}
