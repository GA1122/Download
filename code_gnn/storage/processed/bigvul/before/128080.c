SynchronousCompositorImpl* SynchronousCompositorImpl::FromRoutingID(
    int routing_id) {
  if (g_factory == nullptr)
    return nullptr;
  if (g_process_id == ChildProcessHost::kInvalidUniqueID)
    return nullptr;
  RenderViewHost* rvh = RenderViewHost::FromID(g_process_id, routing_id);
  if (!rvh)
    return nullptr;
  RenderWidgetHostViewAndroid* rwhva =
      static_cast<RenderWidgetHostViewAndroid*>(rvh->GetWidget()->GetView());
  if (!rwhva)
    return nullptr;
  return rwhva->GetSynchronousCompositorImpl();
}
