void MetalayerMode::OnMetalayerSessionComplete() {
  delegate()->DisableTool(GetToolId());
}
