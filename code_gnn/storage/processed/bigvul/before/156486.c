void RenderFrameDevToolsAgentHost::UpdateRendererChannel(bool force) {
  blink::mojom::DevToolsAgentAssociatedPtr agent_ptr;
  if (frame_host_ && render_frame_alive_ && force)
    frame_host_->GetRemoteAssociatedInterfaces()->GetInterface(&agent_ptr);
  int process_id = frame_host_ ? frame_host_->GetProcess()->GetID()
                               : ChildProcessHost::kInvalidUniqueID;
  GetRendererChannel()->SetRenderer(std::move(agent_ptr), process_id,
                                    frame_host_);
}
