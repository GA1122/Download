static bool IsVaapiSupportedJpeg(const JpegParseResult& jpeg) {
  if (!VaapiWrapper::IsJpegDecodingSupportedForInternalFormat(
          VaSurfaceFormatForJpeg(jpeg.frame_header))) {
    DLOG(ERROR) << "The JPEG's subsampling format is unsupported";
    return false;
  }

  if (jpeg.frame_header.visible_width == 0u) {
    DLOG(ERROR) << "Visible width can't be zero";
    return false;
  }
  if (jpeg.frame_header.visible_height == 0u) {
    DLOG(ERROR) << "Visible height can't be zero";
    return false;
  }

  gfx::Size min_jpeg_resolution;
  if (!VaapiWrapper::GetJpegDecodeMinResolution(&min_jpeg_resolution)) {
    DLOG(ERROR) << "Could not get the minimum resolution";
    return false;
  }
  gfx::Size max_jpeg_resolution;
  if (!VaapiWrapper::GetJpegDecodeMaxResolution(&max_jpeg_resolution)) {
    DLOG(ERROR) << "Could not get the maximum resolution";
    return false;
  }
  const int actual_jpeg_coded_width =
      base::strict_cast<int>(jpeg.frame_header.coded_width);
  const int actual_jpeg_coded_height =
      base::strict_cast<int>(jpeg.frame_header.coded_height);
  if (actual_jpeg_coded_width < min_jpeg_resolution.width() ||
      actual_jpeg_coded_height < min_jpeg_resolution.height() ||
      actual_jpeg_coded_width > max_jpeg_resolution.width() ||
      actual_jpeg_coded_height > max_jpeg_resolution.height()) {
    DLOG(ERROR) << "VAAPI doesn't support size " << actual_jpeg_coded_width
                << "x" << actual_jpeg_coded_height << ": not in range "
                << min_jpeg_resolution.ToString() << " - "
                << max_jpeg_resolution.ToString();
    return false;
  }

  return true;
}
