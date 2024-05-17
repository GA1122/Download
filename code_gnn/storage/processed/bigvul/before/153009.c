void FormatStringWithHyphens(base::string16* text) {
  struct HyphenPosition {
    HyphenPosition() : position(0), next_whitespace_position(0) {}
    size_t position;
    size_t next_whitespace_position;   
  };
  std::vector<HyphenPosition> hyphen_positions;
  HyphenPosition current_hyphen_position;
  bool current_hyphen_position_is_valid = false;
  const base::char16 kPdfiumHyphenEOL = 0xfffe;

  for (size_t i = 0; i < text->size(); ++i) {
    const base::char16& current_char = (*text)[i];
    if (current_char == kPdfiumHyphenEOL) {
      if (current_hyphen_position_is_valid)
        hyphen_positions.push_back(current_hyphen_position);
      current_hyphen_position = HyphenPosition();
      current_hyphen_position.position = i;
      current_hyphen_position_is_valid = true;
    } else if (base::IsUnicodeWhitespace(current_char)) {
      if (current_hyphen_position_is_valid) {
        if (current_char != L'\r' && current_char != L'\n')
          current_hyphen_position.next_whitespace_position = i;
        hyphen_positions.push_back(current_hyphen_position);
        current_hyphen_position_is_valid = false;
      }
    }
  }
  if (current_hyphen_position_is_valid)
    hyphen_positions.push_back(current_hyphen_position);

  while (!hyphen_positions.empty()) {
    static const base::char16 kCr[] = {L'\r', L'\0'};
    const HyphenPosition& position = hyphen_positions.back();
    if (position.next_whitespace_position != 0) {
      (*text)[position.next_whitespace_position] = L'\n';
      text->insert(position.next_whitespace_position, kCr);
    }
    text->erase(position.position, 1);
    hyphen_positions.pop_back();
  }

  static const base::char16 kSpaceCrCn[] = {L' ', L'\r', L'\n', L'\0'};
  static const base::char16 kCrCn[] = {L'\r', L'\n', L'\0'};
  base::ReplaceSubstringsAfterOffset(text, 0, kSpaceCrCn, kCrCn);
}
