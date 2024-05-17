Address ObjectStartBitmap::FindHeader(
    Address address_maybe_pointing_to_the_middle_of_object) {
  size_t object_offset =
      address_maybe_pointing_to_the_middle_of_object - offset_;
  size_t object_start_number = object_offset / kAllocationGranularity;
  size_t cell_index = object_start_number / kCellSize;
#if DCHECK_IS_ON()
  const size_t bitmap_size = kReservedForBitmap;
  DCHECK_LT(cell_index, bitmap_size);
#endif
  size_t bit = object_start_number & kCellMask;
  uint8_t byte = object_start_bit_map_[cell_index] & ((1 << (bit + 1)) - 1);
  while (!byte) {
    DCHECK_LT(0u, cell_index);
    byte = object_start_bit_map_[--cell_index];
  }
  int leading_zeroes = base::bits::CountLeadingZeroBits(byte);
  object_start_number =
      (cell_index * kCellSize) + (kCellSize - 1) - leading_zeroes;
  object_offset = object_start_number * kAllocationGranularity;
  return object_offset + offset_;
}
