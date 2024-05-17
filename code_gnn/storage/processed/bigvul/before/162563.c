const char* Resource::ResourceTypeToString(
    Type type,
    const AtomicString& fetch_initiator_name) {
  switch (type) {
    case Resource::kMainResource:
      return "Main resource";
    case Resource::kImage:
      return "Image";
    case Resource::kCSSStyleSheet:
      return "CSS stylesheet";
    case Resource::kScript:
      return "Script";
    case Resource::kFont:
      return "Font";
    case Resource::kRaw:
      return InitiatorTypeNameToString(fetch_initiator_name);
    case Resource::kSVGDocument:
      return "SVG document";
    case Resource::kXSLStyleSheet:
      return "XSL stylesheet";
    case Resource::kLinkPrefetch:
      return "Link prefetch resource";
    case Resource::kTextTrack:
      return "Text track";
    case Resource::kImportResource:
      return "Imported resource";
    case Resource::kAudio:
      return "Audio";
    case Resource::kVideo:
      return "Video";
    case Resource::kManifest:
      return "Manifest";
    case Resource::kMock:
      return "Mock";
  }
  NOTREACHED();
  return InitiatorTypeNameToString(fetch_initiator_name);
}
