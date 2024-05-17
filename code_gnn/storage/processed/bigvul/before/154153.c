uint32_t GetCompressedFormatDepthPitch(const CompressedFormatInfo& info,
                                       uint32_t width,
                                       uint32_t height) {
  uint32_t num_blocks_high = (height + info.block_size - 1) / info.block_size;
  return num_blocks_high * GetCompressedFormatRowPitch(info, width);
}
