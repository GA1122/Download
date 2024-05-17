bool Document::IsPrefetchOnly() const {
  if (!frame_ || !frame_->GetPage())
    return false;

  PrerendererClient* prerenderer_client =
      PrerendererClient::From(frame_->GetPage());
  return prerenderer_client && prerenderer_client->IsPrefetchOnly();
}
