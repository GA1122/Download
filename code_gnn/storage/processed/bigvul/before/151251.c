static void MaybeEncodeTextContent(const String& text_content,
                                   const char* buffer_data,
                                   size_t buffer_size,
                                   String* result,
                                   bool* base64_encoded) {
  if (!text_content.IsNull() &&
      !text_content.Utf8(WTF::kStrictUTF8Conversion).IsNull()) {
    *result = text_content;
    *base64_encoded = false;
  } else if (buffer_data) {
    *result = Base64Encode(buffer_data, buffer_size);
    *base64_encoded = true;
  } else if (text_content.IsNull()) {
    *result = "";
    *base64_encoded = false;
  } else {
    DCHECK(!text_content.Is8Bit());
    *result = Base64Encode(text_content.Utf8(WTF::kLenientUTF8Conversion));
    *base64_encoded = true;
  }
}
