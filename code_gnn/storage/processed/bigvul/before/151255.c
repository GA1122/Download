String InspectorPageAgent::ResourceTypeJson(
    InspectorPageAgent::ResourceType resource_type) {
  switch (resource_type) {
    case kDocumentResource:
      return protocol::Page::ResourceTypeEnum::Document;
    case kFontResource:
      return protocol::Page::ResourceTypeEnum::Font;
    case kImageResource:
      return protocol::Page::ResourceTypeEnum::Image;
    case kMediaResource:
      return protocol::Page::ResourceTypeEnum::Media;
    case kScriptResource:
      return protocol::Page::ResourceTypeEnum::Script;
    case kStylesheetResource:
      return protocol::Page::ResourceTypeEnum::Stylesheet;
    case kTextTrackResource:
      return protocol::Page::ResourceTypeEnum::TextTrack;
    case kXHRResource:
      return protocol::Page::ResourceTypeEnum::XHR;
    case kFetchResource:
      return protocol::Page::ResourceTypeEnum::Fetch;
    case kEventSourceResource:
      return protocol::Page::ResourceTypeEnum::EventSource;
    case kWebSocketResource:
      return protocol::Page::ResourceTypeEnum::WebSocket;
    case kManifestResource:
      return protocol::Page::ResourceTypeEnum::Manifest;
    case kOtherResource:
      return protocol::Page::ResourceTypeEnum::Other;
  }
   return protocol::Page::ResourceTypeEnum::Other;
 }
