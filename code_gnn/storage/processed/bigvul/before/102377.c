void AppendFormattedHost(const GURL& url,
                         const std::string& languages,
                         string16* output) {
  std::vector<size_t> offsets;
  AppendFormattedComponent(url.possibly_invalid_spec(),
      url.parsed_for_possibly_invalid_spec().host, offsets,
      HostComponentTransform(languages), output, NULL, NULL);
}
