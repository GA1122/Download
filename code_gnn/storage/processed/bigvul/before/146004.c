WebGL2RenderingContextBase::GetPackPixelStoreParams() {
  WebGLImageConversion::PixelStoreParams params;
  params.alignment = pack_alignment_;
  params.row_length = pack_row_length_;
  params.skip_pixels = pack_skip_pixels_;
  params.skip_rows = pack_skip_rows_;
  return params;
}
