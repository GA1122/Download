ImageData* BaseRenderingContext2D::createImageData(
    ImageData* image_data,
    ExceptionState& exception_state) const {
  ImageData* result = nullptr;
  ImageDataColorSettings color_settings =
      GetColorSettingsAsImageDataColorSettings();
  result = ImageData::Create(image_data->Size(), &color_settings);
  if (!result)
    exception_state.ThrowRangeError("Out of memory at ImageData creation");
  return result;
}
