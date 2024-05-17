void TemplateURL::EncodeSearchTerms(
    const TemplateURLRef::SearchTermsArgs& search_terms_args,
    bool is_in_query,
    std::string* input_encoding,
    base::string16* encoded_terms,
    base::string16* encoded_original_query) const {

  std::vector<std::string> encodings(input_encodings());
  if (std::find(encodings.begin(), encodings.end(), "UTF-8") == encodings.end())
    encodings.push_back("UTF-8");
  for (std::vector<std::string>::const_iterator i(encodings.begin());
       i != encodings.end(); ++i) {
    if (TryEncoding(search_terms_args.search_terms,
                    search_terms_args.original_query, i->c_str(),
                    is_in_query, encoded_terms, encoded_original_query)) {
      *input_encoding = *i;
      return;
    }
  }
  NOTREACHED();
}
