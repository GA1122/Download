void InspectorNetworkAgent::ScriptImported(unsigned long identifier,
                                           const String& source_string) {
  resources_data_->SetResourceContent(IdentifiersFactory::RequestId(identifier),
                                      source_string);
}
