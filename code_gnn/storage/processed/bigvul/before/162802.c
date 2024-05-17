PixelStoreParams ContextState::GetUnpackParams(Dimension dimension) {
  DCHECK_EQ(0, unpack_skip_pixels);
  DCHECK_EQ(0, unpack_skip_rows);
  DCHECK_EQ(0, unpack_skip_images);
  PixelStoreParams params;
  params.alignment = unpack_alignment;
  params.row_length = unpack_row_length;
  if (dimension == k3D) {
    params.image_height = unpack_image_height;
  }
  return params;
}
