void SynchronousCompositorImpl::DidChangeRootLayerScrollOffset(
    const gfx::ScrollOffset& root_offset) {
  DCHECK(CalledOnValidThread());
  if (!synchronous_input_handler_proxy_)
    return;
  synchronous_input_handler_proxy_->SynchronouslySetRootScrollOffset(
      root_offset);
}
