bool TryEncoding(const base::string16& terms,
                 const base::string16& original_query,
                 const char* encoding,
                 bool is_in_query,
                 base::string16* escaped_terms,
                 base::string16* escaped_original_query) {
  DCHECK(escaped_terms);
  DCHECK(escaped_original_query);
  std::string encoded_terms;
  if (!base::UTF16ToCodepage(terms, encoding,
      base::OnStringConversionError::SKIP, &encoded_terms))
    return false;
  *escaped_terms = base::UTF8ToUTF16(is_in_query ?
      net::EscapeQueryParamValue(encoded_terms, true) :
      net::EscapePath(encoded_terms));
  if (original_query.empty())
    return true;
  std::string encoded_original_query;
  if (!base::UTF16ToCodepage(original_query, encoding,
      base::OnStringConversionError::SKIP, &encoded_original_query))
    return false;
  *escaped_original_query = base::UTF8ToUTF16(
      net::EscapeQueryParamValue(encoded_original_query, true));
  return true;
}
