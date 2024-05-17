base::string16 FormatUrlWithOffsets(
    const GURL& url,
    FormatUrlTypes format_types,
    net::UnescapeRule::Type unescape_rules,
    url::Parsed* new_parsed,
    size_t* prefix_end,
    std::vector<size_t>* offsets_for_adjustment) {
  base::OffsetAdjuster::Adjustments adjustments;
  const base::string16& format_url_return_value =
      FormatUrlWithAdjustments(url, format_types, unescape_rules, new_parsed,
                               prefix_end, &adjustments);
  base::OffsetAdjuster::AdjustOffsets(adjustments, offsets_for_adjustment);
  if (offsets_for_adjustment) {
    std::for_each(
        offsets_for_adjustment->begin(), offsets_for_adjustment->end(),
        base::LimitOffset<std::string>(format_url_return_value.length()));
  }
  return format_url_return_value;
}
