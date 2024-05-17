String BuildElementErrorMessage(const String& error) {
  DEFINE_STATIC_LOCAL(const String, element_error_prefix,
                      ("MEDIA_ELEMENT_ERROR: "));
  StringBuilder builder;
  builder.Append(element_error_prefix);
  builder.Append(error);
  return builder.ToString();
}
