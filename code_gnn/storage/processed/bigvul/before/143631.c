RenderWidgetHostImpl* RenderWidgetHostImpl::FromID(
    int32_t process_id,
    int32_t routing_id) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  RoutingIDWidgetMap* widgets = g_routing_id_widget_map.Pointer();
  RoutingIDWidgetMap::iterator it = widgets->find(
      RenderWidgetHostID(process_id, routing_id));
  return it == widgets->end() ? NULL : it->second;
}
