bool InspectorPageAgent::CachedResourceContent(Resource* cached_resource,
                                               String* result,
                                               bool* base64_encoded) {
  bool has_zero_size;
  if (!PrepareResourceBuffer(cached_resource, &has_zero_size))
    return false;

  if (!HasTextContent(cached_resource)) {
    RefPtr<const SharedBuffer> buffer = has_zero_size
                                            ? SharedBuffer::Create()
                                            : cached_resource->ResourceBuffer();
    if (!buffer)
      return false;

    const SharedBuffer::DeprecatedFlatData flat_buffer(std::move(buffer));
    *result = Base64Encode(flat_buffer.Data(), flat_buffer.size());
    *base64_encoded = true;
    return true;
  }

  if (has_zero_size) {
    *result = "";
    *base64_encoded = false;
    return true;
  }

  DCHECK(cached_resource);
  switch (cached_resource->GetType()) {
    case Resource::kCSSStyleSheet:
      MaybeEncodeTextContent(
          ToCSSStyleSheetResource(cached_resource)
              ->SheetText(CSSStyleSheetResource::MIMETypeCheck::kLax),
          cached_resource->ResourceBuffer(), result, base64_encoded);
      return true;
    case Resource::kScript:
      MaybeEncodeTextContent(
          cached_resource->ResourceBuffer()
              ? ToScriptResource(cached_resource)->DecodedText()
              : ToScriptResource(cached_resource)->SourceText(),
          cached_resource->ResourceBuffer(), result, base64_encoded);
      return true;
    default:
      String text_encoding_name =
          cached_resource->GetResponse().TextEncodingName();
      if (text_encoding_name.IsEmpty() &&
          cached_resource->GetType() != Resource::kRaw)
        text_encoding_name = "WinLatin1";
      return InspectorPageAgent::SharedBufferContent(
          cached_resource->ResourceBuffer(),
          cached_resource->GetResponse().MimeType(), text_encoding_name, result,
          base64_encoded);
  }
}
