base::string16 FormatViewSourceUrl(
    const GURL& url,
    FormatUrlTypes format_types,
    net::UnescapeRule::Type unescape_rules,
    url::Parsed* new_parsed,
    size_t* prefix_end,
    base::OffsetAdjuster::Adjustments* adjustments) {
  DCHECK(new_parsed);
  const char kViewSource[] = "view-source:";
  const size_t kViewSourceLength = arraysize(kViewSource) - 1;

  const std::string& url_str(url.possibly_invalid_spec());
  adjustments->clear();
  base::string16 result(
      base::ASCIIToUTF16(kViewSource) +
      FormatUrlWithAdjustments(GURL(url_str.substr(kViewSourceLength)),
                               format_types, unescape_rules, new_parsed,
                               prefix_end, adjustments));
  for (base::OffsetAdjuster::Adjustments::iterator it = adjustments->begin();
       it != adjustments->end(); ++it)
    it->original_offset += kViewSourceLength;

  if (new_parsed->scheme.is_nonempty()) {
    new_parsed->scheme.len += kViewSourceLength;
  } else {
    new_parsed->scheme.begin = 0;
    new_parsed->scheme.len = kViewSourceLength - 1;
  }
  AdjustAllComponentsButScheme(kViewSourceLength, new_parsed);

  if (prefix_end)
    *prefix_end += kViewSourceLength;

  return result;
}
