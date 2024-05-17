void LimitOffsets(const string16& str,
                  std::vector<size_t>* offsets_for_adjustment) {
  if (offsets_for_adjustment) {
    std::for_each(offsets_for_adjustment->begin(),
                  offsets_for_adjustment->end(),
                  LimitOffset<string16>(str.length()));
  }
}
