void AutocompleteInput::ParseForEmphasizeComponents(
    const string16& text,
    const string16& desired_tld,
    url_parse::Component* scheme,
    url_parse::Component* host) {
  url_parse::Parsed parts;
  string16 scheme_str;
  Parse(text, desired_tld, &parts, &scheme_str, NULL);

  *scheme = parts.scheme;
  *host = parts.host;

  int after_scheme_and_colon = parts.scheme.end() + 1;
  if (LowerCaseEqualsASCII(scheme_str, chrome::kViewSourceScheme) &&
      (static_cast<int>(text.length()) > after_scheme_and_colon)) {
    string16 real_url(text.substr(after_scheme_and_colon));
    url_parse::Parsed real_parts;
    AutocompleteInput::Parse(real_url, desired_tld, &real_parts, NULL,
                             NULL);
    if (real_parts.scheme.is_nonempty() || real_parts.host.is_nonempty()) {
      if (real_parts.scheme.is_nonempty()) {
        *scheme = url_parse::Component(
            after_scheme_and_colon + real_parts.scheme.begin,
            real_parts.scheme.len);
      } else {
        scheme->reset();
      }
      if (real_parts.host.is_nonempty()) {
        *host = url_parse::Component(
            after_scheme_and_colon + real_parts.host.begin,
            real_parts.host.len);
      } else {
        host->reset();
      }
    }
  } else if (LowerCaseEqualsASCII(scheme_str, chrome::kFileSystemScheme) &&
             parts.inner_parsed() && parts.inner_parsed()->scheme.is_valid()) {
    *host = parts.inner_parsed()->host;
  }
}
