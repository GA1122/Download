void RenderFrameHostImpl::BindNFCRequest(device::mojom::NFCRequest request) {
  if (delegate_)
    delegate_->GetNFC(std::move(request));
}
