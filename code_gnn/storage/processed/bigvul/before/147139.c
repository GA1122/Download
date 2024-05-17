Resource* DocumentLoader::StartPreload(Resource::Type type,
                                       FetchParameters& params) {
  Resource* resource = nullptr;
  switch (type) {
    case Resource::kImage:
      if (frame_)
        frame_->MaybeAllowImagePlaceholder(params);
      resource = ImageResource::Fetch(params, Fetcher());
      break;
    case Resource::kScript:
      resource = ScriptResource::Fetch(params, Fetcher());
      break;
    case Resource::kCSSStyleSheet:
      resource = CSSStyleSheetResource::Fetch(params, Fetcher());
      break;
    case Resource::kFont:
      resource = FontResource::Fetch(params, Fetcher());
      break;
    case Resource::kMedia:
      resource = RawResource::FetchMedia(params, Fetcher());
      break;
    case Resource::kTextTrack:
      resource = RawResource::FetchTextTrack(params, Fetcher());
      break;
    case Resource::kImportResource:
      resource = RawResource::FetchImport(params, Fetcher());
      break;
    case Resource::kRaw:
      resource = RawResource::Fetch(params, Fetcher());
      break;
    default:
      NOTREACHED();
  }

  return resource;
}
