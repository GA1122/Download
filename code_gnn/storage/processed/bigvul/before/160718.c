void RenderFrameImpl::RunScriptsAtDocumentReady(bool document_is_empty) {
  base::WeakPtr<RenderFrameImpl> weak_self = weak_factory_.GetWeakPtr();

  GetContentClient()->renderer()->RunScriptsAtDocumentEnd(this);

  if (!weak_self.get())
    return;

  if (!document_is_empty)
    return;

  InternalDocumentStateData* internal_data =
      InternalDocumentStateData::FromDocumentLoader(
          frame_->GetDocumentLoader());
  int http_status_code = internal_data->http_status_code();
  if (GetContentClient()->renderer()->HasErrorPage(http_status_code)) {
    LoadNavigationErrorPageForHttpStatusError(
        frame_->GetDocumentLoader()->GetRequest(), frame_->GetDocument().Url(),
        http_status_code, true, nullptr);
  }
}
