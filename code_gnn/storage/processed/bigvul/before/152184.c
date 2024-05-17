void RenderFrameImpl::AbortCommitNavigation(
    mojom::FrameNavigationControl::CommitNavigationCallback callback,
    blink::mojom::CommitResult reason) {
  DCHECK(callback || IsPerNavigationMojoInterfaceEnabled());
  if (callback) {
    std::move(callback).Run(reason);
  } else {
    navigation_client_impl_.reset();
  }
}
