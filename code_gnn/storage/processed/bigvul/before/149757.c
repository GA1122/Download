base::string16 FormatUrlWithAdjustments(
    const GURL& url,
    FormatUrlTypes format_types,
    net::UnescapeRule::Type unescape_rules,
    url::Parsed* new_parsed,
    size_t* prefix_end,
    base::OffsetAdjuster::Adjustments* adjustments) {
  DCHECK(adjustments != NULL);
  adjustments->clear();
  url::Parsed parsed_temp;
  if (!new_parsed)
    new_parsed = &parsed_temp;
  else
    *new_parsed = url::Parsed();

  const char kViewSource[] = "view-source";
  const char kViewSourceTwice[] = "view-source:view-source:";
  if (url.SchemeIs(kViewSource) &&
      !base::StartsWith(url.possibly_invalid_spec(), kViewSourceTwice,
                        base::CompareCase::INSENSITIVE_ASCII)) {
    return FormatViewSourceUrl(url, format_types, unescape_rules,
                               new_parsed, prefix_end, adjustments);
  }

  const std::string& spec = url.possibly_invalid_spec();
  const url::Parsed& parsed = url.parsed_for_possibly_invalid_spec();

  base::string16 url_string;
  url_string.insert(
      url_string.end(), spec.begin(),
      spec.begin() + parsed.CountCharactersBefore(url::Parsed::USERNAME, true));
  const char kHTTP[] = "http://";
  const char kFTP[] = "ftp.";
  bool omit_http =
      (format_types & kFormatUrlOmitHTTP) &&
      base::EqualsASCII(url_string, kHTTP) &&
      !base::StartsWith(url.host(), kFTP, base::CompareCase::SENSITIVE);
  new_parsed->scheme = parsed.scheme;

  if ((format_types & kFormatUrlOmitUsernamePassword) != 0) {
    new_parsed->username.reset();
    new_parsed->password.reset();
    if (parsed.username.is_nonempty() || parsed.password.is_nonempty()) {
      if (parsed.username.is_nonempty() && parsed.password.is_nonempty()) {
        adjustments->push_back(base::OffsetAdjuster::Adjustment(
            static_cast<size_t>(parsed.username.begin),
            static_cast<size_t>(parsed.username.len + parsed.password.len + 2),
            0));
      } else {
        const url::Component* nonempty_component =
            parsed.username.is_nonempty() ? &parsed.username : &parsed.password;
        adjustments->push_back(base::OffsetAdjuster::Adjustment(
            static_cast<size_t>(nonempty_component->begin),
            static_cast<size_t>(nonempty_component->len + 1), 0));
      }
    }
  } else {
    AppendFormattedComponent(spec, parsed.username,
                             NonHostComponentTransform(unescape_rules),
                             &url_string, &new_parsed->username, adjustments);
    if (parsed.password.is_valid())
      url_string.push_back(':');
    AppendFormattedComponent(spec, parsed.password,
                             NonHostComponentTransform(unescape_rules),
                             &url_string, &new_parsed->password, adjustments);
    if (parsed.username.is_valid() || parsed.password.is_valid())
      url_string.push_back('@');
  }
  if (prefix_end)
    *prefix_end = static_cast<size_t>(url_string.length());

  AppendFormattedComponent(spec, parsed.host, HostComponentTransform(),
                           &url_string, &new_parsed->host, adjustments);

  if (parsed.port.is_nonempty()) {
    url_string.push_back(':');
    new_parsed->port.begin = url_string.length();
    url_string.insert(url_string.end(), spec.begin() + parsed.port.begin,
                      spec.begin() + parsed.port.end());
    new_parsed->port.len = url_string.length() - new_parsed->port.begin;
  } else {
    new_parsed->port.reset();
  }

  if (!(format_types & kFormatUrlOmitTrailingSlashOnBareHostname) ||
      !CanStripTrailingSlash(url)) {
    AppendFormattedComponent(spec, parsed.path,
                             NonHostComponentTransform(unescape_rules),
                             &url_string, &new_parsed->path, adjustments);
  } else {
    if (parsed.path.len > 0) {
      adjustments->push_back(base::OffsetAdjuster::Adjustment(
          parsed.path.begin, parsed.path.len, 0));
    }
  }
  if (parsed.query.is_valid())
    url_string.push_back('?');
  AppendFormattedComponent(spec, parsed.query,
                           NonHostComponentTransform(unescape_rules),
                           &url_string, &new_parsed->query, adjustments);

  if (parsed.ref.is_valid())
    url_string.push_back('#');
  AppendFormattedComponent(spec, parsed.ref,
                           NonHostComponentTransform(net::UnescapeRule::NONE),
                           &url_string, &new_parsed->ref, adjustments);

  if (omit_http && base::StartsWith(url_string, base::ASCIIToUTF16(kHTTP),
                                    base::CompareCase::SENSITIVE)) {
    const size_t kHTTPSize = arraysize(kHTTP) - 1;
    url_string = url_string.substr(kHTTPSize);
    adjustments->insert(adjustments->begin(),
                        base::OffsetAdjuster::Adjustment(0, kHTTPSize, 0));

    if (prefix_end)
      *prefix_end -= kHTTPSize;

    DCHECK(new_parsed->scheme.is_valid());
    int delta = -(new_parsed->scheme.len + 3);   
    new_parsed->scheme.reset();
    AdjustAllComponentsButScheme(delta, new_parsed);
  }

  return url_string;
}
