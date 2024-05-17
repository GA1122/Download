Resource* DocumentLoader::StartPreload(ResourceType type,
                                       FetchParameters& params) {
  Resource* resource = nullptr;
  switch (type) {
    case ResourceType::kImage:
      resource = ImageResource::Fetch(params, Fetcher());
      break;
    case ResourceType::kScript:
      params.SetRequestContext(mojom::RequestContextType::SCRIPT);
      resource = ScriptResource::Fetch(params, Fetcher(), nullptr,
                                       ScriptResource::kAllowStreaming);
      break;
    case ResourceType::kCSSStyleSheet:
      resource = CSSStyleSheetResource::Fetch(params, Fetcher(), nullptr);
      break;
    case ResourceType::kFont:
      resource = FontResource::Fetch(params, Fetcher(), nullptr);
      break;
    case ResourceType::kAudio:
    case ResourceType::kVideo:
      resource = RawResource::FetchMedia(params, Fetcher(), nullptr);
      break;
    case ResourceType::kTextTrack:
      resource = RawResource::FetchTextTrack(params, Fetcher(), nullptr);
      break;
    case ResourceType::kImportResource:
      resource = RawResource::FetchImport(params, Fetcher(), nullptr);
      break;
    case ResourceType::kRaw:
      resource = RawResource::Fetch(params, Fetcher(), nullptr);
      break;
    default:
      NOTREACHED();
  }

  return resource;
}
