void ResourceFetcher::UpdateAllImageResourcePriorities() {
  TRACE_EVENT0(
      "blink",
      "ResourceLoadPriorityOptimizer::updateAllImageResourcePriorities");
  for (const auto& document_resource : document_resources_) {
    Resource* resource = document_resource.value.Get();
    if (!resource || !resource->IsImage() || !resource->IsLoading())
      continue;

    ResourcePriority resource_priority = resource->PriorityFromObservers();
    ResourceLoadPriority resource_load_priority =
        ComputeLoadPriority(Resource::kImage, resource->GetResourceRequest(),
                            resource_priority.visibility);
    if (resource_load_priority == resource->GetResourceRequest().Priority())
      continue;

    resource->DidChangePriority(resource_load_priority,
                                resource_priority.intra_priority_value);
    network_instrumentation::resourcePrioritySet(resource->Identifier(),
                                                 resource_load_priority);
    Context().DispatchDidChangeResourcePriority(
        resource->Identifier(), resource_load_priority,
        resource_priority.intra_priority_value);
  }
}