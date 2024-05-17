void AppendFormattedHost(const GURL& url, base::string16* output) {
  AppendFormattedComponent(
      url.possibly_invalid_spec(), url.parsed_for_possibly_invalid_spec().host,
      HostComponentTransform(), output, NULL, NULL);
}
