base::string16 TemplateURLRef::SearchTermToString16(
    const std::string& term) const {
  const std::vector<std::string>& encodings = owner_->input_encodings();
  base::string16 result;

  net::UnescapeRule::Type unescape_rules =
      net::UnescapeRule::SPACES | net::UnescapeRule::PATH_SEPARATORS |
      net::UnescapeRule::URL_SPECIAL_CHARS_EXCEPT_PATH_SEPARATORS;
  if (search_term_key_location_ != url::Parsed::PATH)
    unescape_rules |= net::UnescapeRule::REPLACE_PLUS_WITH_SPACE;

  std::string unescaped = net::UnescapeURLComponent(term, unescape_rules);
  for (size_t i = 0; i < encodings.size(); ++i) {
    if (base::CodepageToUTF16(unescaped, encodings[i].c_str(),
                              base::OnStringConversionError::FAIL, &result))
      return result;
  }

  if (base::CodepageToUTF16(unescaped, base::kCodepageUTF8,
                            base::OnStringConversionError::FAIL, &result))
    return result;

  result = base::UTF8ToUTF16(term);
  if (unescape_rules & net::UnescapeRule::REPLACE_PLUS_WITH_SPACE)
    std::replace(result.begin(), result.end(), '+', ' ');
  return result;
}
