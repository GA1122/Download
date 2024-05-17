ImageData* BaseRenderingContext2D::createImageData(
    ImageDataArray& data_array,
    unsigned width,
    unsigned height,
    ExceptionState& exception_state) const {
  ImageDataColorSettings color_settings;
  return ImageData::CreateImageData(data_array, width, height, color_settings,
                                    exception_state);
}
