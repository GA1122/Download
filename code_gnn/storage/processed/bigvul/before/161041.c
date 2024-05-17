base::string16 FormatUrlWithAdjustments(
    const GURL& url,
    FormatUrlTypes format_types,
    net::UnescapeRule::Type unescape_rules,
    url::Parsed* new_parsed,
    size_t* prefix_end,
    base::OffsetAdjuster::Adjustments* adjustments) {
  DCHECK(adjustments);
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

  size_t scheme_size = static_cast<size_t>(parsed.CountCharactersBefore(
      url::Parsed::USERNAME, true  ));
  base::string16 url_string;
  url_string.insert(url_string.end(), spec.begin(), spec.begin() + scheme_size);
  new_parsed->scheme = parsed.scheme;

  if (((format_types & kFormatUrlOmitUsernamePassword) != 0) ||
      ((format_types & kFormatUrlTrimAfterHost) != 0)) {
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

  bool trim_trivial_subdomains =
      (format_types & kFormatUrlOmitTrivialSubdomains) != 0;
  AppendFormattedComponent(spec, parsed.host,
                           HostComponentTransform(trim_trivial_subdomains),
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

  if ((format_types & kFormatUrlTrimAfterHost) && url.IsStandard() &&
      !url.SchemeIsFile() && !url.SchemeIsFileSystem()) {
    size_t trimmed_length = parsed.path.len;
    if (parsed.query.is_valid())
      trimmed_length += parsed.query.len + 1;

    if (parsed.ref.is_valid())
      trimmed_length += parsed.ref.len + 1;

    adjustments->push_back(
        base::OffsetAdjuster::Adjustment(parsed.path.begin, trimmed_length, 0));

  } else if ((format_types & kFormatUrlOmitTrailingSlashOnBareHostname) &&
             CanStripTrailingSlash(url)) {
    if (parsed.path.len > 0) {
      adjustments->push_back(base::OffsetAdjuster::Adjustment(
          parsed.path.begin, parsed.path.len, 0));
    }
  } else {
    AppendFormattedComponent(spec, parsed.path,
                             NonHostComponentTransform(unescape_rules),
                             &url_string, &new_parsed->path, adjustments);

    if (parsed.query.is_valid())
      url_string.push_back('?');
    AppendFormattedComponent(spec, parsed.query,
                             NonHostComponentTransform(unescape_rules),
                             &url_string, &new_parsed->query, adjustments);

    if (parsed.ref.is_valid())
      url_string.push_back('#');
    AppendFormattedComponent(spec, parsed.ref,
                             NonHostComponentTransform(unescape_rules),
                             &url_string, &new_parsed->ref, adjustments);
  }

  const char kFTP[] = "ftp.";
  bool strip_scheme =
      !base::StartsWith(url.host(), kFTP, base::CompareCase::SENSITIVE) &&
      (((format_types & kFormatUrlOmitHTTP) &&
        url.SchemeIs(url::kHttpScheme)) ||
       ((format_types & kFormatUrlOmitHTTPS) &&
        url.SchemeIs(url::kHttpsScheme)) ||
       ((format_types & kFormatUrlOmitFileScheme) &&
        url.SchemeIs(url::kFileScheme)) ||
       ((format_types & kFormatUrlOmitMailToScheme) &&
        url.SchemeIs(url::kMailToScheme)));

  if (strip_scheme) {
    DCHECK(new_parsed->scheme.is_valid());
    size_t scheme_and_separator_len =
        url.SchemeIs(url::kMailToScheme)
            ? new_parsed->scheme.len + 1    
            : new_parsed->scheme.len + 3;   
#if defined(OS_WIN)
    if (url.SchemeIs(url::kFileScheme) &&
        base::StartsWith(url_string, base::ASCIIToUTF16("file:///"),
                         base::CompareCase::INSENSITIVE_ASCII)) {
      ++new_parsed->path.begin;
      ++scheme_size;
      ++scheme_and_separator_len;
    }
#endif

    url_string.erase(0, scheme_size);
    adjustments->insert(adjustments->begin(),
                        base::OffsetAdjuster::Adjustment(0, scheme_size, 0));

    if (prefix_end)
      *prefix_end -= scheme_size;

    new_parsed->scheme.reset();
    AdjustAllComponentsButScheme(-scheme_and_separator_len, new_parsed);
  }

  return url_string;
}
