bool TemplateURL::IsGoogleSearchURLWithReplaceableKeyword(
    const SearchTermsData& search_terms_data) const {
  return (type_ == NORMAL) &&
      url_ref_->HasGoogleBaseURLs(search_terms_data) &&
      google_util::IsGoogleHostname(base::UTF16ToUTF8(data_.keyword()),
                                    google_util::DISALLOW_SUBDOMAIN);
}
