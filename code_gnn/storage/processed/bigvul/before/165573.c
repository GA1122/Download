void DocumentLoader::DispatchLinkHeaderPreloads(
    ViewportDescriptionWrapper* viewport,
    LinkLoader::MediaPreloadPolicy media_policy) {
  DCHECK_GE(state_, kCommitted);
  LinkLoader::LoadLinksFromHeader(
      GetResponse().HttpHeaderField(http_names::kLink), GetResponse().Url(),
      *frame_, frame_->GetDocument(), NetworkHintsInterfaceImpl(),
      LinkLoader::kOnlyLoadResources, media_policy, viewport);
}
