void AdjustForComponentTransform(
    const std::vector<size_t>& original_offsets,
    size_t original_component_begin,
    size_t original_component_end,
    const std::vector<size_t>& transformed_offsets,
    size_t output_component_begin,
    std::vector<size_t>* offsets_for_adjustment) {
  if (!offsets_for_adjustment)
    return;

  DCHECK_NE(std::string::npos, original_component_begin);
  DCHECK_NE(std::string::npos, original_component_end);
  DCHECK_NE(string16::npos, output_component_begin);
  size_t offsets_size = offsets_for_adjustment->size();
  DCHECK_EQ(offsets_size, original_offsets.size());
  DCHECK_EQ(offsets_size, transformed_offsets.size());
  for (size_t i = 0; i < offsets_size; ++i) {
    size_t original_offset = original_offsets[i];
    if ((original_offset >= original_component_begin) &&
        (original_offset < original_component_end)) {
      size_t transformed_offset = transformed_offsets[i];
      (*offsets_for_adjustment)[i] = (transformed_offset == string16::npos) ?
          string16::npos : (output_component_begin + transformed_offset);
    }
  }
}
