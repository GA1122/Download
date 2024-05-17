void RenderFrameHostImpl::CopyImageAt(int x, int y) {
  gfx::PointF point_in_view =
      GetView()->TransformRootPointToViewCoordSpace(gfx::PointF(x, y));
  Send(new FrameMsg_CopyImageAt(routing_id_, point_in_view.x(),
                                point_in_view.y()));
}
