DocumentLoader* FrameLoader::CreateDocumentLoader(
    const ResourceRequest& request,
    const SubstituteData& substitute_data,
    ClientRedirectPolicy client_redirect_policy,
    const base::UnguessableToken& devtools_navigation_token,
    WebFrameLoadType load_type,
    WebNavigationType navigation_type,
    std::unique_ptr<WebNavigationParams> navigation_params,
    std::unique_ptr<WebDocumentLoader::ExtraData> extra_data) {
  DocumentLoader* loader = Client()->CreateDocumentLoader(
      frame_, request,
      substitute_data.IsValid() ? substitute_data
                                : DefaultSubstituteDataForURL(request.Url()),
      client_redirect_policy, devtools_navigation_token, load_type,
      navigation_type, std::move(navigation_params), std::move(extra_data));
  probe::lifecycleEvent(frame_, loader, "init", CurrentTimeTicksInSeconds());
  return loader;
}
