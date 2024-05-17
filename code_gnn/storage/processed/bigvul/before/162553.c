bool Resource::MustReloadDueToVaryHeader(
    const ResourceRequest& new_request) const {
  const AtomicString& vary = GetResponse().HttpHeaderField(HTTPNames::Vary);
  if (vary.IsNull())
    return false;
  if (vary == "*")
    return true;

  CommaDelimitedHeaderSet vary_headers;
  ParseCommaDelimitedHeader(vary, vary_headers);
  for (const String& header : vary_headers) {
    AtomicString atomic_header(header);
    if (GetResourceRequest().HttpHeaderField(atomic_header) !=
        new_request.HttpHeaderField(atomic_header)) {
      return true;
    }
  }
  return false;
}
