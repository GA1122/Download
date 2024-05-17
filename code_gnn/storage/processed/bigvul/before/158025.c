DocumentLoader* LocalFrameClientImpl::CreateDocumentLoader(
    LocalFrame* frame,
    const ResourceRequest& request,
    const SubstituteData& data,
    ClientRedirectPolicy client_redirect_policy,
    const base::UnguessableToken& devtools_navigation_token,
    std::unique_ptr<WebNavigationParams> navigation_params,
    std::unique_ptr<WebDocumentLoader::ExtraData> extra_data) {
  DCHECK(frame);

  WebDocumentLoaderImpl* document_loader = WebDocumentLoaderImpl::Create(
      frame, request, data, client_redirect_policy, devtools_navigation_token);
  SetupDocumentLoader(document_loader, std::move(navigation_params));
  document_loader->SetExtraData(std::move(extra_data));
  if (web_frame_->Client())
    web_frame_->Client()->DidCreateDocumentLoader(document_loader);
  return document_loader;
}
