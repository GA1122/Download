std::vector<size_t> OffsetsIntoComponent(
    const std::vector<size_t>& original_offsets,
    size_t component_begin) {
  DCHECK_NE(std::string::npos, component_begin);
  std::vector<size_t> offsets_into_component(original_offsets);
  for (std::vector<size_t>::iterator i(offsets_into_component.begin());
       i != offsets_into_component.end(); ++i) {
    if (*i != std::string::npos)
      *i = (*i < component_begin) ? std::string::npos : (*i - component_begin);
  }
  return offsets_into_component;
}
