AtomicString GetTypeExtension(
    Document* document,
    const StringOrElementCreationOptions& string_or_options) {
  if (string_or_options.IsNull())
    return AtomicString();

  if (string_or_options.IsString()) {
    UseCounter::Count(document,
                      WebFeature::kDocumentCreateElement2ndArgStringHandling);
    return AtomicString(string_or_options.GetAsString());
  }

  if (string_or_options.IsElementCreationOptions()) {
    const ElementCreationOptions& options =
        *string_or_options.GetAsElementCreationOptions();
    if (options.hasIs())
      return AtomicString(options.is());
  }

  return AtomicString();
}
