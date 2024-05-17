static bool hasTextContent(Resource* cachedResource)
{
    InspectorPageAgent::ResourceType type = InspectorPageAgent::cachedResourceType(*cachedResource);
    return type == InspectorPageAgent::DocumentResource || type == InspectorPageAgent::StylesheetResource || type == InspectorPageAgent::ScriptResource || type == InspectorPageAgent::XHRResource;
}
