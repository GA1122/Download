void RenderFrameDevToolsAgentHost::OnPageScaleFactorChanged(
    float page_scale_factor) {
  for (auto* input : protocol::InputHandler::ForAgentHost(this))
    input->OnPageScaleFactorChanged(page_scale_factor);
}
