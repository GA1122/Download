void ReportContentTypeResultToUMA(String content_type,
                                  MIMETypeRegistry::SupportsType result) {
  DEFINE_THREAD_SAFE_STATIC_LOCAL(
      EnumerationHistogram, content_type_parseable_histogram,
      ("Media.MediaElement.ContentTypeParseable", kContentTypeParseableMax));
  ParsedContentType parsed_content_type(content_type);
  ContentTypeParseableResult uma_result = kIsNotSupportedNotParseable;
  switch (result) {
    case MIMETypeRegistry::kIsSupported:
      uma_result = parsed_content_type.IsValid() ? kIsSupportedParseable
                                                 : kIsSupportedNotParseable;
      break;
    case MIMETypeRegistry::kMayBeSupported:
      uma_result = parsed_content_type.IsValid() ? kMayBeSupportedParseable
                                                 : kMayBeSupportedNotParseable;
      break;
    case MIMETypeRegistry::kIsNotSupported:
      uma_result = parsed_content_type.IsValid() ? kIsNotSupportedParseable
                                                 : kIsNotSupportedNotParseable;
      break;
  }
  content_type_parseable_histogram.Count(uma_result);
}
