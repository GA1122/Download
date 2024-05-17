static void MaybeEncodeTextContent(const String& text_content,
                                   RefPtr<const SharedBuffer> buffer,
                                   String* result,
                                   bool* base64_encoded) {
  if (!buffer) {
    return MaybeEncodeTextContent(text_content, nullptr, 0, result,
                                  base64_encoded);
  }

  const SharedBuffer::DeprecatedFlatData flat_buffer(std::move(buffer));
  return MaybeEncodeTextContent(text_content, flat_buffer.Data(),
                                flat_buffer.size(), result, base64_encoded);
}
