void LocalFrame::PauseSubresourceLoading(
    blink::mojom::blink::PauseSubresourceLoadingHandleRequest request) {
  auto handle = GetFrameScheduler()->GetPauseSubresourceLoadingHandle();
  if (!handle)
    return;
  pause_handle_bindings_.AddBinding(std::move(handle), std::move(request));
}
