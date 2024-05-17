bool ImageResource::HasClientsOrObservers() const {
  return Resource::HasClientsOrObservers() || GetContent()->HasObservers();
}
