base::string16 FormatUrl(const GURL& url,
                         FormatUrlTypes format_types,
                         net::UnescapeRule::Type unescape_rules,
                         url::Parsed* new_parsed,
                         size_t* prefix_end,
                         size_t* offset_for_adjustment) {
  base::OffsetAdjuster::Adjustments adjustments;
  base::string16 result = FormatUrlWithAdjustments(
      url, format_types, unescape_rules, new_parsed, prefix_end, &adjustments);
  if (offset_for_adjustment) {
    base::OffsetAdjuster::AdjustOffset(adjustments, offset_for_adjustment,
                                       result.length());
  }
  return result;
}
