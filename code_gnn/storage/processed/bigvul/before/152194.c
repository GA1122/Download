void RenderFrameImpl::BindDevToolsAgent(
    blink::mojom::DevToolsAgentHostAssociatedPtrInfo host,
    blink::mojom::DevToolsAgentAssociatedRequest request) {
  frame_->BindDevToolsAgent(host.PassHandle(), request.PassHandle());
}
