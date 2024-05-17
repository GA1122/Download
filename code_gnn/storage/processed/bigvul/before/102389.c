string16 FormatUrlWithOffsets(const GURL& url,
                              const std::string& languages,
                              FormatUrlTypes format_types,
                              UnescapeRule::Type unescape_rules,
                              url_parse::Parsed* new_parsed,
                              size_t* prefix_end,
                              std::vector<size_t>* offsets_for_adjustment) {
  url_parse::Parsed parsed_temp;
  if (!new_parsed)
    new_parsed = &parsed_temp;
  else
    *new_parsed = url_parse::Parsed();
  std::vector<size_t> original_offsets;
  if (offsets_for_adjustment)
    original_offsets = *offsets_for_adjustment;

  const char* const kViewSource = "view-source";
  const char* const kViewSourceTwice = "view-source:view-source:";
  if (url.SchemeIs(kViewSource) &&
      !StartsWithASCII(url.possibly_invalid_spec(), kViewSourceTwice, false)) {
    return FormatViewSourceUrl(url, original_offsets, languages, format_types,
        unescape_rules, new_parsed, prefix_end, offsets_for_adjustment);
  }

  const std::string& spec = url.possibly_invalid_spec();
  const url_parse::Parsed& parsed = url.parsed_for_possibly_invalid_spec();

  string16 url_string;
  url_string.insert(url_string.end(), spec.begin(),
      spec.begin() + parsed.CountCharactersBefore(url_parse::Parsed::USERNAME,
                                                  true));
  const char kHTTP[] = "http://";
  const char kFTP[] = "ftp.";
  bool omit_http = (format_types & kFormatUrlOmitHTTP) &&
      EqualsASCII(url_string, kHTTP) &&
      !StartsWithASCII(url.host(), kFTP, true);
  new_parsed->scheme = parsed.scheme;

  if ((format_types & kFormatUrlOmitUsernamePassword) != 0) {
    new_parsed->username.reset();
    new_parsed->password.reset();
    if (offsets_for_adjustment && !offsets_for_adjustment->empty() &&
        (parsed.username.is_nonempty() || parsed.password.is_nonempty())) {
      OffsetAdjuster offset_adjuster(offsets_for_adjustment);
      if (parsed.username.is_nonempty() && parsed.password.is_nonempty()) {
        offset_adjuster.Add(OffsetAdjuster::Adjustment(
            static_cast<size_t>(parsed.username.begin),
            static_cast<size_t>(parsed.username.len + parsed.password.len + 2),
            0));
      } else {
        const url_parse::Component* nonempty_component =
            parsed.username.is_nonempty() ? &parsed.username : &parsed.password;
        offset_adjuster.Add(OffsetAdjuster::Adjustment(
            static_cast<size_t>(nonempty_component->begin),
            static_cast<size_t>(nonempty_component->len + 1), 0));
      }
    }
  } else {
    AppendFormattedComponent(spec, parsed.username, original_offsets,
        NonHostComponentTransform(unescape_rules), &url_string,
        &new_parsed->username, offsets_for_adjustment);
    if (parsed.password.is_valid()) {
      size_t colon = parsed.username.end();
      DCHECK_EQ(static_cast<size_t>(parsed.password.begin - 1), colon);
      std::vector<size_t>::const_iterator colon_iter =
          std::find(original_offsets.begin(), original_offsets.end(), colon);
      if (colon_iter != original_offsets.end()) {
        (*offsets_for_adjustment)[colon_iter - original_offsets.begin()] =
            url_string.length();
      }
      url_string.push_back(':');
    }
    AppendFormattedComponent(spec, parsed.password, original_offsets,
        NonHostComponentTransform(unescape_rules), &url_string,
        &new_parsed->password, offsets_for_adjustment);
    if (parsed.username.is_valid() || parsed.password.is_valid()) {
      size_t at_sign = (parsed.password.is_valid() ?
          parsed.password : parsed.username).end();
      DCHECK_EQ(static_cast<size_t>(parsed.host.begin - 1), at_sign);
      std::vector<size_t>::const_iterator at_sign_iter =
          std::find(original_offsets.begin(), original_offsets.end(), at_sign);
      if (at_sign_iter != original_offsets.end()) {
        (*offsets_for_adjustment)[at_sign_iter - original_offsets.begin()] =
            url_string.length();
      }
      url_string.push_back('@');
    }
  }
  if (prefix_end)
    *prefix_end = static_cast<size_t>(url_string.length());

  AppendFormattedComponent(spec, parsed.host, original_offsets,
      HostComponentTransform(languages), &url_string, &new_parsed->host,
      offsets_for_adjustment);

  if (parsed.port.is_nonempty()) {
    url_string.push_back(':');
    new_parsed->port.begin = url_string.length();
    url_string.insert(url_string.end(),
                      spec.begin() + parsed.port.begin,
                      spec.begin() + parsed.port.end());
    new_parsed->port.len = url_string.length() - new_parsed->port.begin;
  } else {
    new_parsed->port.reset();
  }

  if (!(format_types & kFormatUrlOmitTrailingSlashOnBareHostname) ||
      !CanStripTrailingSlash(url)) {
    AppendFormattedComponent(spec, parsed.path, original_offsets,
        NonHostComponentTransform(unescape_rules), &url_string,
        &new_parsed->path, offsets_for_adjustment);
  }
  if (parsed.query.is_valid())
    url_string.push_back('?');
  AppendFormattedComponent(spec, parsed.query, original_offsets,
      NonHostComponentTransform(unescape_rules), &url_string,
      &new_parsed->query, offsets_for_adjustment);

  if (parsed.ref.is_valid()) {
    url_string.push_back('#');
    size_t original_ref_begin = static_cast<size_t>(parsed.ref.begin);
    size_t output_ref_begin = url_string.length();
    new_parsed->ref.begin = static_cast<int>(output_ref_begin);

    std::vector<size_t> offsets_into_ref(
        OffsetsIntoComponent(original_offsets, original_ref_begin));
    if (parsed.ref.len > 0) {
      url_string.append(UTF8ToUTF16AndAdjustOffsets(
          spec.substr(original_ref_begin, static_cast<size_t>(parsed.ref.len)),
          &offsets_into_ref));
    }

    new_parsed->ref.len =
        static_cast<int>(url_string.length() - new_parsed->ref.begin);
    AdjustForComponentTransform(original_offsets, original_ref_begin,
        static_cast<size_t>(parsed.ref.end()), offsets_into_ref,
        output_ref_begin, offsets_for_adjustment);
  }

  if (omit_http && StartsWith(url_string, ASCIIToUTF16(kHTTP), true)) {
    const size_t kHTTPSize = arraysize(kHTTP) - 1;
    url_string = url_string.substr(kHTTPSize);
    if (offsets_for_adjustment && !offsets_for_adjustment->empty()) {
      OffsetAdjuster offset_adjuster(offsets_for_adjustment);
      offset_adjuster.Add(OffsetAdjuster::Adjustment(0, kHTTPSize, 0));
    }
    if (prefix_end)
      *prefix_end -= kHTTPSize;

    DCHECK(new_parsed->scheme.is_valid());
    int delta = -(new_parsed->scheme.len + 3);   
    new_parsed->scheme.reset();
    AdjustComponents(delta, new_parsed);
  }

  LimitOffsets(url_string, offsets_for_adjustment);
  return url_string;
}
