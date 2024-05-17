static std::unique_ptr<TextResourceDecoder> CreateResourceTextDecoder(
    const String& mime_type,
    const String& text_encoding_name) {
  if (!text_encoding_name.IsEmpty()) {
    return TextResourceDecoder::Create(TextResourceDecoderOptions(
        TextResourceDecoderOptions::kPlainTextContent,
        WTF::TextEncoding(text_encoding_name)));
  }
  if (DOMImplementation::IsXMLMIMEType(mime_type)) {
    TextResourceDecoderOptions options(TextResourceDecoderOptions::kXMLContent);
    options.SetUseLenientXMLDecoding();
    return TextResourceDecoder::Create(options);
  }
  if (DeprecatedEqualIgnoringCase(mime_type, "text/html")) {
    return TextResourceDecoder::Create(TextResourceDecoderOptions(
        TextResourceDecoderOptions::kHTMLContent, UTF8Encoding()));
  }
  if (MIMETypeRegistry::IsSupportedJavaScriptMIMEType(mime_type) ||
      DOMImplementation::IsJSONMIMEType(mime_type)) {
    return TextResourceDecoder::Create(TextResourceDecoderOptions(
        TextResourceDecoderOptions::kPlainTextContent, UTF8Encoding()));
  }
  if (DOMImplementation::IsTextMIMEType(mime_type)) {
    return TextResourceDecoder::Create(TextResourceDecoderOptions(
        TextResourceDecoderOptions::kPlainTextContent,
        WTF::TextEncoding("ISO-8859-1")));
  }
  return std::unique_ptr<TextResourceDecoder>();
}
