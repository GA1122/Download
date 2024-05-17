base::string16 FormatUrlWithOffsets(
    const GURL& url,
    FormatUrlTypes format_types,
    net::UnescapeRule::Type unescape_rules,
    url::Parsed* new_parsed,
    size_t* prefix_end,
    std::vector<size_t>* offsets_for_adjustment) {
  base::OffsetAdjuster::Adjustments adjustments;
  const base::string16& result = FormatUrlWithAdjustments(
      url, format_types, unescape_rules, new_parsed, prefix_end, &adjustments);
  base::OffsetAdjuster::AdjustOffsets(adjustments, offsets_for_adjustment,
                                      result.length());
  return result;
}
