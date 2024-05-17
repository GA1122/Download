void RenderFrameHostImpl::DidCommitProvisionalLoad(
    std::unique_ptr<FrameHostMsg_DidCommitProvisionalLoad_Params>
        validated_params,
    mojom::DidCommitProvisionalLoadInterfaceParamsPtr interface_params) {
  if (MaybeInterceptCommitCallback(nullptr, validated_params.get(),
                                   &interface_params)) {
    DidCommitNavigation(std::move(navigation_request_),
                        std::move(validated_params),
                        std::move(interface_params));
  }
}
