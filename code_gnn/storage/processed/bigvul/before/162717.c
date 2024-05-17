ImageData* BaseRenderingContext2D::createImageData(
    int sw,
    int sh,
    ExceptionState& exception_state) const {
  if (!sw || !sh) {
    exception_state.ThrowDOMException(
        kIndexSizeError,
        String::Format("The source %s is 0.", sw ? "height" : "width"));
    return nullptr;
  }

  IntSize size(abs(sw), abs(sh));
  ImageData* result = nullptr;
  ImageDataColorSettings color_settings =
      GetColorSettingsAsImageDataColorSettings();
  result = ImageData::Create(size, &color_settings);

  if (!result)
    exception_state.ThrowRangeError("Out of memory at ImageData creation");
  return result;
}
