void RenderProcessHostImpl::Attach(RenderWidgetHost* host,
                                   int routing_id) {
  render_widget_hosts_.AddWithID(host, routing_id);
}
