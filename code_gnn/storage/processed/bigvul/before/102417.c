string16 IDNToUnicodeWithOffsets(const std::string& host,
                                 const std::string& languages,
                                 std::vector<size_t>* offsets_for_adjustment) {
  string16 input16;
  input16.reserve(host.length());
  input16.insert(input16.end(), host.begin(), host.end());

  string16 out16;
  {
    OffsetAdjuster offset_adjuster(offsets_for_adjustment);
    for (size_t component_start = 0, component_end;
         component_start < input16.length();
         component_start = component_end + 1) {
      component_end = input16.find('.', component_start);
      if (component_end == string16::npos)
        component_end = input16.length();   
      size_t component_length = component_end - component_start;
      size_t new_component_start = out16.length();
      bool converted_idn = false;
      if (component_end > component_start) {
        converted_idn = IDNToUnicodeOneComponent(
            input16.data() + component_start, component_length, languages,
            &out16);
      }
      size_t new_component_length = out16.length() - new_component_start;

      if (converted_idn && offsets_for_adjustment) {
        offset_adjuster.Add(OffsetAdjuster::Adjustment(component_start,
            component_length, new_component_length));
      }

      if (component_end < input16.length())
        out16.push_back('.');
    }
  }

  LimitOffsets(out16, offsets_for_adjustment);
  return out16;
}
