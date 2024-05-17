WebURLRequest RenderFrameImpl::CreateURLRequestForCommit(
    const CommonNavigationParams& common_params,
    const RequestNavigationParams& request_params,
    network::mojom::URLLoaderClientEndpointsPtr url_loader_client_endpoints,
    const network::ResourceResponseHead& head) {
  std::unique_ptr<NavigationResponseOverrideParameters> response_override(
      new NavigationResponseOverrideParameters());
  response_override->url_loader_client_endpoints =
      std::move(url_loader_client_endpoints);
  response_override->response = head;
  response_override->redirect_responses = request_params.redirect_response;
  response_override->redirect_infos = request_params.redirect_infos;

  WebURLRequest request = CreateURLRequestForNavigation(
      common_params, request_params, std::move(response_override),
      frame_->IsViewSourceModeEnabled());
  request.SetFrameType(IsTopLevelNavigation(frame_)
                           ? network::mojom::RequestContextFrameType::kTopLevel
                           : network::mojom::RequestContextFrameType::kNested);
  request.SetRequestorID(render_view_->GetRoutingID());
  static_cast<RequestExtraData*>(request.GetExtraData())
      ->set_render_frame_id(routing_id_);

#if defined(OS_ANDROID)
  request.SetHasUserGesture(common_params.has_user_gesture);
#endif

  request.SetNavigationStartTime(common_params.navigation_start);

  return request;
}
