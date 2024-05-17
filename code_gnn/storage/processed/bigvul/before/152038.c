void RenderFrameHostImpl::OnPortalActivated(
    const base::UnguessableToken& portal_token,
    blink::mojom::PortalAssociatedPtrInfo portal,
    blink::TransferableMessage data,
    base::OnceCallback<void(bool)> callback) {
  GetNavigationControl()->OnPortalActivated(
      portal_token, std::move(portal), std::move(data), std::move(callback));
}
