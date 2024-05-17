content::ResourceType GetResourceType(content::ResourceType resource_type,
                                      const std::string& mime_type) {
  if (resource_type == content::ResourceType::kPrefetch ||
      resource_type == content::ResourceType::kSubResource ||
      resource_type == content::ResourceType::kXhr) {
    return GetResourceTypeFromMimeType(mime_type,
                                       content::ResourceType::kSubResource);
  }
  return resource_type;
}
