InspectorPageAgent::BuildObjectForFrameTree(LocalFrame* frame) {
  std::unique_ptr<protocol::Page::Frame> frame_object =
      BuildObjectForFrame(frame);
  std::unique_ptr<protocol::Array<protocol::Page::FrameResource>> subresources =
      protocol::Array<protocol::Page::FrameResource>::create();

  HeapVector<Member<Resource>> all_resources =
      CachedResourcesForFrame(frame, true);
  for (Resource* cached_resource : all_resources) {
    std::unique_ptr<protocol::Page::FrameResource> resource_object =
        protocol::Page::FrameResource::create()
            .setUrl(UrlWithoutFragment(cached_resource->Url()).GetString())
            .setType(CachedResourceTypeJson(*cached_resource))
            .setMimeType(cached_resource->GetResponse().MimeType())
            .setContentSize(cached_resource->GetResponse().DecodedBodyLength())
            .build();
    double last_modified = cached_resource->GetResponse().LastModified();
    if (!std::isnan(last_modified))
      resource_object->setLastModified(last_modified);
    if (cached_resource->WasCanceled())
      resource_object->setCanceled(true);
    else if (cached_resource->GetStatus() == ResourceStatus::kLoadError)
      resource_object->setFailed(true);
    subresources->addItem(std::move(resource_object));
  }

  HeapVector<Member<Document>> all_imports =
      InspectorPageAgent::ImportsForFrame(frame);
  for (Document* import : all_imports) {
    std::unique_ptr<protocol::Page::FrameResource> resource_object =
        protocol::Page::FrameResource::create()
            .setUrl(UrlWithoutFragment(import->Url()).GetString())
            .setType(ResourceTypeJson(InspectorPageAgent::kDocumentResource))
            .setMimeType(import->SuggestedMIMEType())
            .build();
    subresources->addItem(std::move(resource_object));
  }

  std::unique_ptr<protocol::Page::FrameResourceTree> result =
      protocol::Page::FrameResourceTree::create()
          .setFrame(std::move(frame_object))
          .setResources(std::move(subresources))
          .build();

  std::unique_ptr<protocol::Array<protocol::Page::FrameResourceTree>>
      children_array;
  for (Frame* child = frame->Tree().FirstChild(); child;
       child = child->Tree().NextSibling()) {
    if (!child->IsLocalFrame())
      continue;
    if (!children_array)
      children_array =
          protocol::Array<protocol::Page::FrameResourceTree>::create();
    children_array->addItem(BuildObjectForFrameTree(ToLocalFrame(child)));
  }
  result->setChildFrames(std::move(children_array));
  return result;
}
