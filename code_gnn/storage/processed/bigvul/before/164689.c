void RenderFrameHostImpl::DidCommitPerNavigationMojoInterfaceNavigation(
    NavigationRequest* committing_navigation_request,
    std::unique_ptr<FrameHostMsg_DidCommitProvisionalLoad_Params>
        validated_params,
    mojom::DidCommitProvisionalLoadInterfaceParamsPtr interface_params) {
  DCHECK(committing_navigation_request);
  committing_navigation_request->IgnoreCommitInterfaceDisconnection();
  if (!MaybeInterceptCommitCallback(committing_navigation_request,
                                    validated_params.get(),
                                    &interface_params)) {
    return;
  }
  auto request = navigation_requests_.find(committing_navigation_request);

  CHECK(request != navigation_requests_.end());

  DidCommitNavigation(std::move(request->second), std::move(validated_params),
                      std::move(interface_params));
}
