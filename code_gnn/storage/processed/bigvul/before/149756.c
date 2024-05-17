base::string16 FormatUrl(const GURL& url,
                         FormatUrlTypes format_types,
                         net::UnescapeRule::Type unescape_rules,
                         url::Parsed* new_parsed,
                         size_t* prefix_end,
                         size_t* offset_for_adjustment) {
  std::vector<size_t> offsets;
  if (offset_for_adjustment)
    offsets.push_back(*offset_for_adjustment);
  base::string16 result =
      FormatUrlWithOffsets(url, format_types, unescape_rules, new_parsed,
                           prefix_end, &offsets);
  if (offset_for_adjustment)
    *offset_for_adjustment = offsets[0];
  return result;
}
