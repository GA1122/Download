void InspectorNetworkAgent::DidReceiveScriptResponse(unsigned long identifier) {
  resources_data_->SetResourceType(IdentifiersFactory::RequestId(identifier),
                                   InspectorPageAgent::kScriptResource);
}
