string16 FormatUrl(const GURL& url,
                   const std::string& languages,
                   FormatUrlTypes format_types,
                   UnescapeRule::Type unescape_rules,
                   url_parse::Parsed* new_parsed,
                   size_t* prefix_end,
                   size_t* offset_for_adjustment) {
  std::vector<size_t> offsets;
  if (offset_for_adjustment)
    offsets.push_back(*offset_for_adjustment);
  string16 result = FormatUrlWithOffsets(url, languages, format_types,
      unescape_rules, new_parsed, prefix_end, &offsets);
  if (offset_for_adjustment)
    *offset_for_adjustment = offsets[0];
  return result;
}
