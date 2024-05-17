Resource* DocumentLoader::StartPreload(Resource::Type type,
                                       FetchParameters& params,
                                       CSSPreloaderResourceClient* client) {
  Resource* resource = nullptr;
  DCHECK(!client || type == Resource::kCSSStyleSheet);
  switch (type) {
    case Resource::kImage:
      if (frame_)
        frame_->MaybeAllowImagePlaceholder(params);
      resource = ImageResource::Fetch(params, Fetcher());
      break;
    case Resource::kScript:
      resource = ScriptResource::Fetch(params, Fetcher(), nullptr);
      break;
    case Resource::kCSSStyleSheet:
      resource = CSSStyleSheetResource::Fetch(params, Fetcher(), client);
      break;
    case Resource::kFont:
      resource = FontResource::Fetch(params, Fetcher(), nullptr);
      break;
    case Resource::kAudio:
    case Resource::kVideo:
      resource = RawResource::FetchMedia(params, Fetcher(), nullptr);
      break;
    case Resource::kTextTrack:
      resource = RawResource::FetchTextTrack(params, Fetcher(), nullptr);
      break;
    case Resource::kImportResource:
      resource = RawResource::FetchImport(params, Fetcher(), nullptr);
      break;
    case Resource::kRaw:
      resource = RawResource::Fetch(params, Fetcher(), nullptr);
      break;
    default:
      NOTREACHED();
  }

  return resource;
}
