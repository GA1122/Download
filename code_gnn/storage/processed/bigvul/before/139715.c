ArchiveResource* ResourceFetcher::CreateArchive(Resource* resource) {
  if (!Context().IsMainFrame())
    return nullptr;
  archive_ = MHTMLArchive::Create(resource->Url(), resource->ResourceBuffer());
  return archive_ ? archive_->MainResource() : nullptr;
}
