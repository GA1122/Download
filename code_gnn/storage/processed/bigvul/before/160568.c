WebURLRequest RenderFrameImpl::CreateURLRequestForCommit(
    const CommonNavigationParams& common_params,
    const RequestNavigationParams& request_params,
    network::mojom::URLLoaderClientEndpointsPtr url_loader_client_endpoints,
    const network::ResourceResponseHead& head,
    const GURL& body_url,
    bool is_same_document_navigation) {
  std::unique_ptr<StreamOverrideParameters> stream_override(
      new StreamOverrideParameters());
  stream_override->stream_url = body_url;
  stream_override->url_loader_client_endpoints =
      std::move(url_loader_client_endpoints);
  stream_override->response = head;
  stream_override->redirects = request_params.redirects;
  stream_override->redirect_responses = request_params.redirect_response;
  stream_override->redirect_infos = request_params.redirect_infos;

  stream_override->on_delete = base::BindOnce(
      [](base::WeakPtr<RenderFrameImpl> weak_self, const GURL& url) {
        if (RenderFrameImpl* self = weak_self.get()) {
          self->Send(
              new FrameHostMsg_StreamHandleConsumed(self->routing_id_, url));
        }
      },
      weak_factory_.GetWeakPtr());

  WebURLRequest request = CreateURLRequestForNavigation(
      common_params, request_params, std::move(stream_override),
      frame_->IsViewSourceModeEnabled(), is_same_document_navigation);
  request.SetFrameType(IsTopLevelNavigation(frame_)
                           ? network::mojom::RequestContextFrameType::kTopLevel
                           : network::mojom::RequestContextFrameType::kNested);

  if (common_params.post_data) {
    request.SetHTTPBody(GetWebHTTPBodyForRequestBody(*common_params.post_data));
    if (!request_params.post_content_type.empty()) {
      request.AddHTTPHeaderField(
          WebString::FromASCII(net::HttpRequestHeaders::kContentType),
          WebString::FromASCII(request_params.post_content_type));
    }
  }

#if defined(OS_ANDROID)
  request.SetHasUserGesture(common_params.has_user_gesture);
#endif

  request.SetCheckForBrowserSideNavigation(false);

  request.SetNavigationStartTime(
      ConvertToBlinkTime(common_params.navigation_start));

  return request;
}
