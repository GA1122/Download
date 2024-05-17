PixelStoreParams ContextState::GetPackParams() {
  DCHECK_EQ(0, pack_skip_pixels);
  DCHECK_EQ(0, pack_skip_rows);
  PixelStoreParams params;
  params.alignment = pack_alignment;
  params.row_length = pack_row_length;
  return params;
}
