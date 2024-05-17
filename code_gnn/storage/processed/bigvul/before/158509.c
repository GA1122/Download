void WebLocalFrameImpl::CommitNavigation(
    const WebURLRequest& request,
    WebFrameLoadType web_frame_load_type,
    const WebHistoryItem& item,
    bool is_client_redirect,
    const base::UnguessableToken& devtools_navigation_token,
    std::unique_ptr<WebDocumentLoader::ExtraData> extra_data,
    const WebNavigationTimings& navigation_timings) {
  DCHECK(GetFrame());
  DCHECK(!request.IsNull());
  DCHECK(!request.Url().ProtocolIs("javascript"));
  const ResourceRequest& resource_request = request.ToResourceRequest();

  if (GetTextFinder())
    GetTextFinder()->ClearActiveFindMatch();

  FrameLoadRequest frame_request =
      FrameLoadRequest(nullptr, resource_request,  AtomicString(),
                       kCheckContentSecurityPolicy, devtools_navigation_token);
  if (is_client_redirect)
    frame_request.SetClientRedirect(ClientRedirectPolicy::kClientRedirect);
  HistoryItem* history_item = item;
  GetFrame()->Loader().CommitNavigation(frame_request, web_frame_load_type,
                                        history_item, std::move(extra_data),
                                        navigation_timings);
}
