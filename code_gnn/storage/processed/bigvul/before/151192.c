void InspectorNetworkAgent::MarkResourceAsCached(unsigned long identifier) {
  GetFrontend()->requestServedFromCache(
      IdentifiersFactory::RequestId(identifier));
}
