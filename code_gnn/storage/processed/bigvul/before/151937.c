void RenderFrameHostImpl::ExecuteMediaPlayerActionAtLocation(
    const gfx::Point& location,
    const blink::WebMediaPlayerAction& action) {
  gfx::PointF point_in_view = GetView()->TransformRootPointToViewCoordSpace(
      gfx::PointF(location.x(), location.y()));
  Send(new FrameMsg_MediaPlayerActionAt(routing_id_, point_in_view, action));
}
