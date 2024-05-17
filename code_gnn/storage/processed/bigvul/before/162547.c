bool Resource::IsLoadEventBlockingResourceType() const {
  switch (type_) {
    case Resource::kMainResource:
    case Resource::kImage:
    case Resource::kCSSStyleSheet:
    case Resource::kScript:
    case Resource::kFont:
    case Resource::kSVGDocument:
    case Resource::kXSLStyleSheet:
    case Resource::kImportResource:
      return true;
    case Resource::kRaw:
    case Resource::kLinkPrefetch:
    case Resource::kTextTrack:
    case Resource::kAudio:
    case Resource::kVideo:
    case Resource::kManifest:
    case Resource::kMock:
      return false;
  }
  NOTREACHED();
  return false;
}
