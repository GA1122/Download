base::string16 TemplateURLRef::DisplayURL(
    const SearchTermsData& search_terms_data) const {
  ParseIfNecessary(search_terms_data);
  std::string result(GetURL());
  if (valid_ && !replacements_.empty()) {
    base::ReplaceSubstringsAfterOffset(&result, 0,
                                       kSearchTermsParameterFull,
                                       kDisplaySearchTerms);
    base::ReplaceSubstringsAfterOffset(&result, 0,
                                       kGoogleUnescapedSearchTermsParameterFull,
                                       kDisplayUnescapedSearchTerms);
  }
  return base::UTF8ToUTF16(result);
}
