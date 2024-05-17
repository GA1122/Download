void RenderFrameHostImpl::DidCommitProvisionalLoadForTesting(
    std::unique_ptr<FrameHostMsg_DidCommitProvisionalLoad_Params> params,
    service_manager::mojom::InterfaceProviderRequest
        interface_provider_request) {
  DidCommitProvisionalLoad(std::move(params),
                           std::move(interface_provider_request));
}
