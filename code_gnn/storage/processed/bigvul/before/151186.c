Response InspectorNetworkAgent::GetResponseBody(const String& request_id,
                                                String* content,
                                                bool* base64_encoded) {
  NetworkResourcesData::ResourceData const* resource_data =
      resources_data_->Data(request_id);
  if (!resource_data) {
    return Response::Error("No resource with given identifier found");
  }

  if (resource_data->HasContent()) {
    *content = resource_data->Content();
    *base64_encoded = resource_data->Base64Encoded();
    return Response::OK();
  }

  if (resource_data->IsContentEvicted()) {
    return Response::Error("Request content was evicted from inspector cache");
  }

  if (resource_data->Buffer() && !resource_data->TextEncodingName().IsNull()) {
    bool success = InspectorPageAgent::SharedBufferContent(
        resource_data->Buffer(), resource_data->MimeType(),
        resource_data->TextEncodingName(), content, base64_encoded);
    DCHECK(success);
    return Response::OK();
  }

  if (resource_data->CachedResource() &&
      InspectorPageAgent::CachedResourceContent(resource_data->CachedResource(),
                                                content, base64_encoded)) {
    return Response::OK();
  }

  return Response::Error("No data found for resource with given identifier");
}
