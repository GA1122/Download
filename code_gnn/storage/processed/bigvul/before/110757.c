void AutocompleteInput::UpdateText(const string16& text,
                                   const url_parse::Parsed& parts) {
  text_ = text;
  parts_ = parts;
}
