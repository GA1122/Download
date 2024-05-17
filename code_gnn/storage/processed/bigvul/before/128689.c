bool TemplateURLRef::ExtractSearchTermsFromURL(
    const GURL& url,
    base::string16* search_terms,
    const SearchTermsData& search_terms_data,
    url::Parsed::ComponentType* search_terms_component,
    url::Component* search_terms_position) const {
  DCHECK(search_terms);
  search_terms->clear();

  ParseIfNecessary(search_terms_data);

  if (search_term_key_.empty() &&
      (search_term_key_location_ != url::Parsed::PATH))
    return false;

  if ((url.host() != host_) ||
      (url.port() != port_) ||
      ((url.path() != path_) &&
          (search_term_key_location_ != url::Parsed::PATH))) {
    return false;
  }

  std::string source;
  url::Component position;

  if (search_term_key_location_ == url::Parsed::PATH) {
    source = url.path();

    if (source.length() < path_.length())
      return false;
    position.begin = search_term_position_in_path_;
    position.len = source.length() - path_.length();
    if (source.substr(0, position.begin) + source.substr(position.end()) !=
        path_)
      return false;
  } else {
    DCHECK(search_term_key_location_ == url::Parsed::QUERY ||
           search_term_key_location_ == url::Parsed::REF);
    source = (search_term_key_location_ == url::Parsed::QUERY) ?
        url.query() : url.ref();

    url::Component query, key, value;
    query.len = static_cast<int>(source.size());
    bool key_found = false;
    while (url::ExtractQueryKeyValue(source.c_str(), &query, &key, &value)) {
      if (key.is_nonempty()) {
        if (source.substr(key.begin, key.len) == search_term_key_) {
          if (key_found)
            return false;

          base::StringPiece search_term =
              base::StringPiece(source).substr(value.begin, value.len);
          if (!search_term.starts_with(search_term_value_prefix_) ||
              !search_term.ends_with(search_term_value_suffix_))
            continue;

          key_found = true;
          position =
              url::MakeRange(value.begin + search_term_value_prefix_.size(),
                             value.end() - search_term_value_suffix_.size());
        }
      }
    }
    if (!key_found)
      return false;
  }

  *search_terms =
      SearchTermToString16(source.substr(position.begin, position.len));
  if (search_terms_component)
    *search_terms_component = search_term_key_location_;
  if (search_terms_position)
    *search_terms_position = position;
  return true;
}
