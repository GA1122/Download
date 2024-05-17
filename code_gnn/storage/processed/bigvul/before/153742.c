PixelStoreParams GLES2Implementation::GetUnpackParameters(Dimension dimension) {
  PixelStoreParams params;
  params.alignment = unpack_alignment_;
  params.row_length = unpack_row_length_;
  params.skip_pixels = unpack_skip_pixels_;
  params.skip_rows = unpack_skip_rows_;
  if (dimension == k3D) {
    params.image_height = unpack_image_height_;
    params.skip_images = unpack_skip_images_;
  }
  return params;
}
