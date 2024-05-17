Response InspectorNetworkAgent::getCertificate(
    const String& origin,
    std::unique_ptr<protocol::Array<String>>* certificate) {
  *certificate = protocol::Array<String>::create();
  RefPtr<SecurityOrigin> security_origin =
      SecurityOrigin::CreateFromString(origin);
  for (auto& resource : resources_data_->Resources()) {
    RefPtr<SecurityOrigin> resource_origin =
        SecurityOrigin::Create(resource->RequestedURL());
    if (resource_origin->IsSameSchemeHostPort(security_origin.get()) &&
        resource->Certificate().size()) {
      for (auto& cert : resource->Certificate())
        certificate->get()->addItem(Base64Encode(cert.Latin1()));
      return Response::OK();
    }
  }
  return Response::OK();
}
