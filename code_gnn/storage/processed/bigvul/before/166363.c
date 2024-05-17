void RenderFrameDevToolsAgentHost::UpdateRendererChannel(bool force) {
  blink::mojom::DevToolsAgentAssociatedPtr agent_ptr;
  blink::mojom::DevToolsAgentHostAssociatedRequest host_request;
  if (frame_host_ && render_frame_alive_ && force) {
    blink::mojom::DevToolsAgentHostAssociatedPtrInfo host_ptr_info;
    host_request = mojo::MakeRequest(&host_ptr_info);
    frame_host_->BindDevToolsAgent(std::move(host_ptr_info),
                                   mojo::MakeRequest(&agent_ptr));
  }
  int process_id = frame_host_ ? frame_host_->GetProcess()->GetID()
                               : ChildProcessHost::kInvalidUniqueID;
  GetRendererChannel()->SetRendererAssociated(
      std::move(agent_ptr), std::move(host_request), process_id, frame_host_);
}
