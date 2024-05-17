void RenderFrameImpl::OnPortalActivated(
    const base::UnguessableToken& portal_token,
    blink::mojom::PortalAssociatedPtrInfo portal,
    blink::TransferableMessage data,
    OnPortalActivatedCallback callback) {
  frame_->OnPortalActivated(portal_token, portal.PassHandle(), std::move(data),
                            std::move(callback));
}
