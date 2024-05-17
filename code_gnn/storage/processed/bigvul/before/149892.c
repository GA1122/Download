void LayerTreeHostImpl::FrameData::AppendRenderPass(
    std::unique_ptr<RenderPass> render_pass) {
  render_passes.push_back(std::move(render_pass));
}
