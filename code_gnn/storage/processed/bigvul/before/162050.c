void RenderProcessHostImpl::BindFrameSinkProvider(
    mojom::FrameSinkProviderRequest request) {
  frame_sink_provider_.Bind(std::move(request));
}
