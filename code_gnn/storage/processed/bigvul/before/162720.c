ImageData* BaseRenderingContext2D::createImageData(
    ImageDataArray& data_array,
    unsigned width,
    unsigned height,
    ImageDataColorSettings& color_settings,
    ExceptionState& exception_state) const {
  return ImageData::CreateImageData(data_array, width, height, color_settings,
                                    exception_state);
}
