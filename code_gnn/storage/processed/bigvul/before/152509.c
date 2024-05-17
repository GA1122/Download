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
    WebDocumentLoader* document_loader = frame_->GetDocumentLoader();
    WebURL unreachable_url = frame_->GetDocument().Url();
    std::string error_html;
    GetContentClient()->renderer()->PrepareErrorPageForHttpStatusError(
        this, unreachable_url, document_loader->HttpMethod().Ascii(),
        false  , http_status_code, &error_html);
    LoadNavigationErrorPage(document_loader,
                            WebURLError(net::ERR_FAILED, unreachable_url),
                            error_html, true  ,
                            false  );
    if (!weak_self)
      return;
  }
}
