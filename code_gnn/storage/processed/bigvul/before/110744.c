AutocompleteInput::Type AutocompleteInput::Parse(
    const string16& text,
    const string16& desired_tld,
    url_parse::Parsed* parts,
    string16* scheme,
    GURL* canonicalized_url) {
  const size_t first_non_white = text.find_first_not_of(kWhitespaceUTF16, 0);
  if (first_non_white == string16::npos)
    return INVALID;   

  if (text.at(first_non_white) == L'?') {
    return FORCED_QUERY;
  }

  url_parse::Parsed local_parts;
  if (!parts)
    parts = &local_parts;
  const string16 parsed_scheme(URLFixerUpper::SegmentURL(text, parts));
  if (scheme)
    *scheme = parsed_scheme;
  if (canonicalized_url) {
    *canonicalized_url = URLFixerUpper::FixupURL(UTF16ToUTF8(text),
                                                 UTF16ToUTF8(desired_tld));
  }

  if (LowerCaseEqualsASCII(parsed_scheme, chrome::kFileScheme)) {
    return URL;
  }

  if (LowerCaseEqualsASCII(parsed_scheme, chrome::kFileSystemScheme)) {
    if (parts->inner_parsed() && parts->inner_parsed()->scheme.is_valid())
      return URL;
  }

  if (parts->scheme.is_nonempty() &&
      !LowerCaseEqualsASCII(parsed_scheme, chrome::kHttpScheme) &&
      !LowerCaseEqualsASCII(parsed_scheme, chrome::kHttpsScheme)) {
    if (ProfileIOData::IsHandledProtocol(UTF16ToASCII(parsed_scheme)))
      return URL;

    if (LowerCaseEqualsASCII(parsed_scheme, chrome::kViewSourceScheme) ||
        LowerCaseEqualsASCII(parsed_scheme, chrome::kJavaScriptScheme) ||
        LowerCaseEqualsASCII(parsed_scheme, chrome::kDataScheme))
      return URL;

    ExternalProtocolHandler::BlockState block_state =
        ExternalProtocolHandler::GetBlockState(UTF16ToUTF8(parsed_scheme));
    switch (block_state) {
      case ExternalProtocolHandler::DONT_BLOCK:
        return URL;

      case ExternalProtocolHandler::BLOCK:
        return QUERY;

      default: {
        const string16 http_scheme_prefix =
            ASCIIToUTF16(std::string(chrome::kHttpScheme) +
                         content::kStandardSchemeSeparator);
        url_parse::Parsed http_parts;
        string16 http_scheme;
        GURL http_canonicalized_url;
        Type http_type = Parse(http_scheme_prefix + text, desired_tld,
                               &http_parts, &http_scheme,
                               &http_canonicalized_url);
        DCHECK_EQ(std::string(chrome::kHttpScheme), UTF16ToUTF8(http_scheme));

        if ((http_type == URL || http_type == REQUESTED_URL) &&
            http_parts.username.is_nonempty() &&
            http_parts.password.is_nonempty()) {
          http_parts.scheme.reset();
          url_parse::Component* components[] = {
            &http_parts.username,
            &http_parts.password,
            &http_parts.host,
            &http_parts.port,
            &http_parts.path,
            &http_parts.query,
            &http_parts.ref,
          };
          for (size_t i = 0; i < arraysize(components); ++i) {
            URLFixerUpper::OffsetComponent(
                -static_cast<int>(http_scheme_prefix.length()), components[i]);
          }

          *parts = http_parts;
          if (scheme)
            scheme->clear();
          if (canonicalized_url)
            *canonicalized_url = http_canonicalized_url;

          return http_type;
        }

        return UNKNOWN;
      }
    }
  }


  if (!parts->host.is_nonempty())
    return QUERY;

  const string16 host(text.substr(parts->host.begin, parts->host.len));
  const size_t registry_length =
      net::RegistryControlledDomainService::GetRegistryLength(UTF16ToUTF8(host),
                                                              false);
  if (registry_length == std::string::npos) {
    if (!desired_tld.empty()) {
      string16 host_with_tld(host);
      if (host[host.length() - 1] != '.')
        host_with_tld += '.';
      host_with_tld += desired_tld;
      if (net::RegistryControlledDomainService::GetRegistryLength(
          UTF16ToUTF8(host_with_tld), false) != std::string::npos)
        return REQUESTED_URL;   
    }
    return QUERY;   
  }


  url_canon::CanonHostInfo host_info;
  const std::string canonicalized_host(net::CanonicalizeHost(UTF16ToUTF8(host),
                                                             &host_info));
  if ((host_info.family == url_canon::CanonHostInfo::NEUTRAL) &&
      !net::IsCanonicalizedHostCompliant(canonicalized_host,
                                         UTF16ToUTF8(desired_tld))) {
    return (parts->scheme.is_nonempty() ||
           ((registry_length != 0) && (host.find(' ') == string16::npos))) ?
        UNKNOWN : QUERY;
  }

  if (url_parse::ParsePort(text.c_str(), parts->port) ==
      url_parse::PORT_INVALID)
    return QUERY;

  if (parts->scheme.is_nonempty())
    return URL;

  if (host_info.family == url_canon::CanonHostInfo::IPV6)
    return URL;
  if ((host_info.family == url_canon::CanonHostInfo::IPV4) &&
      (host_info.num_ipv4_components == 4))
    return URL;

  if (parts->password.is_nonempty())
    return URL;

  if (parts->path.is_nonempty()) {
    char c = text[parts->path.end() - 1];
    if ((c == '\\') || (c == '/'))
      return URL;
  }

  if (NumNonHostComponents(*parts) > 1)
    return URL;

  if ((host_info.family != url_canon::CanonHostInfo::IPV4) &&
      ((registry_length != 0) || (host == ASCIIToUTF16("localhost") ||
       parts->port.is_nonempty())))
    return parts->username.is_nonempty() ? UNKNOWN : URL;

  if (!desired_tld.empty())
    return REQUESTED_URL;

  return UNKNOWN;
}
