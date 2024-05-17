void DocumentLoader::DispatchLinkHeaderPreloads(
    ViewportDescriptionWrapper* viewport,
    LinkLoader::MediaPreloadPolicy media_policy) {
  DCHECK_GE(state_, kCommitted);
  LinkLoader::LoadLinksFromHeader(
      GetResponse().HttpHeaderField(HTTPNames::Link), GetResponse().Url(),
      *frame_, frame_->GetDocument(), NetworkHintsInterfaceImpl(),
      LinkLoader::kOnlyLoadResources, media_policy, viewport);
}
