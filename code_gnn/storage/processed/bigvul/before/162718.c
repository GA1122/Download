ImageData* BaseRenderingContext2D::createImageData(
    unsigned width,
    unsigned height,
    ImageDataColorSettings& color_settings,
    ExceptionState& exception_state) const {
  return ImageData::CreateImageData(width, height, color_settings,
                                    exception_state);
}
