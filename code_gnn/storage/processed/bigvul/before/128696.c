GURL TemplateURL::GenerateSearchURL(
    const SearchTermsData& search_terms_data) const {
  if (!url_ref_->IsValid(search_terms_data))
    return GURL();

  if (!url_ref_->SupportsReplacement(search_terms_data))
    return GURL(url());

  return GURL(url_ref_->ReplaceSearchTerms(
      TemplateURLRef::SearchTermsArgs(
          base::ASCIIToUTF16("blah.blah.blah.blah.blah")),
      search_terms_data, NULL));
}
