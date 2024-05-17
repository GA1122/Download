SearchTermsKeyResult FindSearchTermsKey(const std::string& params) {
  SearchTermsKeyResult result;
  if (params.empty())
    return result;
  url::Component query, key, value;
  query.len = static_cast<int>(params.size());
  while (url::ExtractQueryKeyValue(params.c_str(), &query, &key, &value)) {
    if (key.is_nonempty() && value.is_nonempty()) {
      const base::StringPiece value_string(params.c_str() + value.begin,
                                           value.len);
      if (TryMatchSearchParam(value_string, kSearchTermsParameterFull,
                              &result.value_prefix, &result.value_suffix) ||
          TryMatchSearchParam(value_string,
                              kGoogleUnescapedSearchTermsParameterFull,
                              &result.value_prefix, &result.value_suffix)) {
        result.key = params.substr(key.begin, key.len);
        break;
      }
    }
  }
  return result;
}
