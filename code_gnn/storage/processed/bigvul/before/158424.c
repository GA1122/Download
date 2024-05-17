viz::CompositorFrame MakeDelegatedFrame(float scale_factor,
                                        gfx::Size size,
                                        gfx::Rect damage) {
  viz::CompositorFrame frame;
  frame.metadata.device_scale_factor = scale_factor;
  frame.metadata.begin_frame_ack = viz::BeginFrameAck(0, 1, true);

  std::unique_ptr<viz::RenderPass> pass = viz::RenderPass::Create();
  pass->SetNew(1, gfx::Rect(size), damage, gfx::Transform());
  frame.render_pass_list.push_back(std::move(pass));
  if (!size.IsEmpty()) {
    viz::TransferableResource resource;
    resource.id = 1;
    frame.resource_list.push_back(std::move(resource));
  }
  return frame;
}
