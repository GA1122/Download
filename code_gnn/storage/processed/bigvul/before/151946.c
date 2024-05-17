void RenderFrameHostImpl::ForwardMessageToPortalHost(
    blink::TransferableMessage message,
    const url::Origin& source_origin,
    const base::Optional<url::Origin>& target_origin) {
  if (target_origin) {
    DCHECK(!target_origin->opaque());
    if (target_origin != GetLastCommittedOrigin())
      return;
  }
  GetNavigationControl()->ForwardMessageToPortalHost(
      std::move(message), source_origin, target_origin);
}
