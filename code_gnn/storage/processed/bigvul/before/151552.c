ArchiveResource* ResourceFetcher::CreateArchive(Resource* resource) {
  if (!Context().IsMainFrame()) {
    Context().AddErrorConsoleMessage(
        "Attempted to load a multipart archive into an subframe: " +
            resource->Url().GetString(),
        FetchContext::kJSSource);
    return nullptr;
  }

  archive_ = MHTMLArchive::Create(resource->Url(), resource->ResourceBuffer());
  if (!archive_) {
    Context().AddErrorConsoleMessage(
        "Malformed multipart archive: " + resource->Url().GetString(),
        FetchContext::kJSSource);
    return nullptr;
  }

  return archive_->MainResource();
}
