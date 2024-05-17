BrowserContextIOData* BrowserContextIOData::FromResourceContext(
    content::ResourceContext* resource_context) {
  return static_cast<ResourceContext*>(resource_context)->io_data_;
}
