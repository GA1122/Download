void RenderFrameDevToolsAgentHost::InspectElement(RenderFrameHost* frame_host,
                                                  int x,
                                                  int y) {
  FrameTreeNode* ftn =
      static_cast<RenderFrameHostImpl*>(frame_host)->frame_tree_node();
  RenderFrameDevToolsAgentHost* host =
      static_cast<RenderFrameDevToolsAgentHost*>(GetOrCreateFor(ftn).get());

  gfx::Point point(x, y);
  if (host->frame_host_) {
    if (RenderWidgetHostView* view = host->frame_host_->GetView()) {
      point = gfx::ToRoundedPoint(
          view->TransformRootPointToViewCoordSpace(gfx::PointF(point)));
    }
  }
  host->GetRendererChannel()->InspectElement(point);
}
