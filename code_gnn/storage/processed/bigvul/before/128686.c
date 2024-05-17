std::string TemplateURLRef::DisplayURLToURLRef(
    const base::string16& display_url) {
  std::string result = base::UTF16ToUTF8(display_url);
  base::ReplaceSubstringsAfterOffset(&result, 0,
                                     kDisplaySearchTerms,
                                     kSearchTermsParameterFull);
  base::ReplaceSubstringsAfterOffset(&result, 0,
                                     kDisplayUnescapedSearchTerms,
                                     kGoogleUnescapedSearchTermsParameterFull);
  return result;
}
