const char* ResourceTypeToString(ResourceType resource_type) {
  switch (resource_type) {
    case RESOURCE_TYPE_MAIN_FRAME:
      return protocol::Page::ResourceTypeEnum::Document;
    case RESOURCE_TYPE_SUB_FRAME:
      return protocol::Page::ResourceTypeEnum::Document;
    case RESOURCE_TYPE_STYLESHEET:
      return protocol::Page::ResourceTypeEnum::Stylesheet;
    case RESOURCE_TYPE_SCRIPT:
      return protocol::Page::ResourceTypeEnum::Script;
    case RESOURCE_TYPE_IMAGE:
      return protocol::Page::ResourceTypeEnum::Image;
    case RESOURCE_TYPE_FONT_RESOURCE:
      return protocol::Page::ResourceTypeEnum::Font;
    case RESOURCE_TYPE_SUB_RESOURCE:
      return protocol::Page::ResourceTypeEnum::Other;
    case RESOURCE_TYPE_OBJECT:
      return protocol::Page::ResourceTypeEnum::Other;
    case RESOURCE_TYPE_MEDIA:
      return protocol::Page::ResourceTypeEnum::Media;
    case RESOURCE_TYPE_WORKER:
      return protocol::Page::ResourceTypeEnum::Other;
    case RESOURCE_TYPE_SHARED_WORKER:
      return protocol::Page::ResourceTypeEnum::Other;
    case RESOURCE_TYPE_PREFETCH:
      return protocol::Page::ResourceTypeEnum::Fetch;
    case RESOURCE_TYPE_FAVICON:
      return protocol::Page::ResourceTypeEnum::Other;
    case RESOURCE_TYPE_XHR:
      return protocol::Page::ResourceTypeEnum::XHR;
    case RESOURCE_TYPE_PING:
      return protocol::Page::ResourceTypeEnum::Other;
    case RESOURCE_TYPE_SERVICE_WORKER:
      return protocol::Page::ResourceTypeEnum::Other;
    case RESOURCE_TYPE_CSP_REPORT:
      return protocol::Page::ResourceTypeEnum::Other;
    case RESOURCE_TYPE_PLUGIN_RESOURCE:
      return protocol::Page::ResourceTypeEnum::Other;
    default:
      return protocol::Page::ResourceTypeEnum::Other;
  }
}
