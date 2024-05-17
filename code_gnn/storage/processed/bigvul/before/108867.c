static RenderViewImpl* FromRoutingID(int32 routing_id) {
  return static_cast<RenderViewImpl*>(
      ChildThread::current()->ResolveRoute(routing_id));
}
