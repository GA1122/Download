void RenderFrameHostImpl::BindDevToolsAgent(
    blink::mojom::DevToolsAgentHostAssociatedPtrInfo host,
    blink::mojom::DevToolsAgentAssociatedRequest request) {
  GetNavigationControl()->BindDevToolsAgent(std::move(host),
                                            std::move(request));
}
