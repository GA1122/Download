std::unique_ptr<protocol::Page::Frame> InspectorPageAgent::BuildObjectForFrame(
    LocalFrame* frame) {
  DocumentLoader* loader = frame->Loader().GetDocumentLoader();
  KURL url = loader->GetRequest().Url();
  std::unique_ptr<protocol::Page::Frame> frame_object =
      protocol::Page::Frame::create()
          .setId(IdentifiersFactory::FrameId(frame))
          .setLoaderId(IdentifiersFactory::LoaderId(loader))
          .setUrl(UrlWithoutFragment(url).GetString())
          .setMimeType(frame->Loader().GetDocumentLoader()->MimeType())
          .setSecurityOrigin(SecurityOrigin::Create(url)->ToRawString())
          .build();
  Frame* parent_frame = frame->Tree().Parent();
  if (parent_frame && parent_frame->IsLocalFrame()) {
    frame_object->setParentId(
        IdentifiersFactory::FrameId(ToLocalFrame(parent_frame)));
  }
  if (frame->DeprecatedLocalOwner()) {
    AtomicString name = frame->DeprecatedLocalOwner()->GetNameAttribute();
    if (name.IsEmpty())
      name = frame->DeprecatedLocalOwner()->getAttribute(HTMLNames::idAttr);
    frame_object->setName(name);
  }

  if (loader && !loader->UnreachableURL().IsEmpty())
    frame_object->setUnreachableUrl(loader->UnreachableURL().GetString());
  return frame_object;
}
