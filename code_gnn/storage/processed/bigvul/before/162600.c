 std::unique_ptr<ScopedVAImage> VaapiJpegDecoder::GetImage(
     uint32_t preferred_image_fourcc,
     VaapiImageDecodeStatus* status) {
  if (va_surface_id_ == VA_INVALID_ID) {
    VLOGF(1) << "No decoded JPEG available";
    *status = VaapiImageDecodeStatus::kInvalidState;
    return nullptr;
  }

  uint32_t image_fourcc;
  if (!VaapiWrapper::GetJpegDecodeSuitableImageFourCC(
          va_rt_format_, preferred_image_fourcc, &image_fourcc)) {
    VLOGF(1) << "Cannot determine the output FOURCC";
    *status = VaapiImageDecodeStatus::kCannotGetImage;
    return nullptr;
  }
  VAImageFormat image_format{.fourcc = image_fourcc};
  auto scoped_image =
      vaapi_wrapper_->CreateVaImage(va_surface_id_, &image_format, coded_size_);
  if (!scoped_image) {
    VLOGF(1) << "Cannot get VAImage, FOURCC = "
             << FourccToString(image_format.fourcc);
    *status = VaapiImageDecodeStatus::kCannotGetImage;
    return nullptr;
  }

  *status = VaapiImageDecodeStatus::kSuccess;
  return scoped_image;
}
