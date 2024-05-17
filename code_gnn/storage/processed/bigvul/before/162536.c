void Resource::DidChangePriority(ResourceLoadPriority load_priority,
                                 int intra_priority_value) {
  resource_request_.SetPriority(load_priority, intra_priority_value);
  if (loader_)
    loader_->DidChangePriority(load_priority, intra_priority_value);
}
