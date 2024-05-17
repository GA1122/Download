TypeBuilder::Page::ResourceType::Enum InspectorPageAgent::resourceTypeJson(InspectorPageAgent::ResourceType resourceType)
{
    switch (resourceType) {
    case DocumentResource:
        return TypeBuilder::Page::ResourceType::Document;
    case ImageResource:
        return TypeBuilder::Page::ResourceType::Image;
    case Font:
        return TypeBuilder::Page::ResourceType::Font;
    case StylesheetResource:
        return TypeBuilder::Page::ResourceType::Stylesheet;
    case ScriptResource:
        return TypeBuilder::Page::ResourceType::Script;
    case XHRResource:
        return TypeBuilder::Page::ResourceType::XHR;
    case WebSocketResource:
        return TypeBuilder::Page::ResourceType::WebSocket;
    case OtherResource:
        return TypeBuilder::Page::ResourceType::Other;
    }
    return TypeBuilder::Page::ResourceType::Other;
}
