bool AddInterceptedResourceType(
    const std::string& resource_type,
    base::flat_set<ResourceType>* intercepted_resource_types) {
  if (resource_type == protocol::Page::ResourceTypeEnum::Document) {
    intercepted_resource_types->insert(RESOURCE_TYPE_MAIN_FRAME);
    intercepted_resource_types->insert(RESOURCE_TYPE_SUB_FRAME);
    return true;
  }
  if (resource_type == protocol::Page::ResourceTypeEnum::Stylesheet) {
    intercepted_resource_types->insert(RESOURCE_TYPE_STYLESHEET);
    return true;
  }
  if (resource_type == protocol::Page::ResourceTypeEnum::Image) {
    intercepted_resource_types->insert(RESOURCE_TYPE_IMAGE);
    return true;
  }
  if (resource_type == protocol::Page::ResourceTypeEnum::Media) {
    intercepted_resource_types->insert(RESOURCE_TYPE_MEDIA);
    return true;
  }
  if (resource_type == protocol::Page::ResourceTypeEnum::Font) {
    intercepted_resource_types->insert(RESOURCE_TYPE_FONT_RESOURCE);
    return true;
  }
  if (resource_type == protocol::Page::ResourceTypeEnum::Script) {
    intercepted_resource_types->insert(RESOURCE_TYPE_SCRIPT);
    return true;
  }
  if (resource_type == protocol::Page::ResourceTypeEnum::XHR) {
    intercepted_resource_types->insert(RESOURCE_TYPE_XHR);
    return true;
  }
  if (resource_type == protocol::Page::ResourceTypeEnum::Fetch) {
    intercepted_resource_types->insert(RESOURCE_TYPE_PREFETCH);
    return true;
  }
  if (resource_type == protocol::Page::ResourceTypeEnum::Other) {
    intercepted_resource_types->insert(RESOURCE_TYPE_SUB_RESOURCE);
    intercepted_resource_types->insert(RESOURCE_TYPE_OBJECT);
    intercepted_resource_types->insert(RESOURCE_TYPE_WORKER);
    intercepted_resource_types->insert(RESOURCE_TYPE_SHARED_WORKER);
    intercepted_resource_types->insert(RESOURCE_TYPE_FAVICON);
    intercepted_resource_types->insert(RESOURCE_TYPE_PING);
    intercepted_resource_types->insert(RESOURCE_TYPE_SERVICE_WORKER);
    intercepted_resource_types->insert(RESOURCE_TYPE_CSP_REPORT);
    intercepted_resource_types->insert(RESOURCE_TYPE_PLUGIN_RESOURCE);
    return true;
  }
  return false;
}
