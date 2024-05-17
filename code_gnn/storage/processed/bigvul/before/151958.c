void RenderFrameHostImpl::GetCredentialManager(
    blink::mojom::CredentialManagerRequest request) {
  GetContentClient()->browser()->BindCredentialManagerRequest(
      this, std::move(request));
}
