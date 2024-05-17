content::ResourceType GetResourceTypeFromMimeType(
    const std::string& mime_type,
    content::ResourceType fallback) {
  if (mime_type.empty()) {
    return fallback;
  } else if (blink::IsSupportedImageMimeType(mime_type)) {
    return content::ResourceType::kImage;
  } else if (blink::IsSupportedJavascriptMimeType(mime_type)) {
    return content::ResourceType::kScript;
  } else if (net::MatchesMimeType("text/css", mime_type)) {
    return content::ResourceType::kStylesheet;
  } else {
    bool found =
        std::any_of(std::begin(kFontMimeTypes), std::end(kFontMimeTypes),
                    [&mime_type](const std::string& mime) {
                      return net::MatchesMimeType(mime, mime_type);
                    });
    if (found)
      return content::ResourceType::kFontResource;
  }
  return fallback;
}
