RenderWidgetHost* RenderProcessHostImpl::GetRenderWidgetHostByID(
    int routing_id) {
  return render_widget_hosts_.Lookup(routing_id);
}
