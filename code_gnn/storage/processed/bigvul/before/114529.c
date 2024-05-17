void WebPluginProxy::AcceleratedPluginAllocatedIOSurface(int32 width,
                                                         int32 height,
                                                         uint32 surface_id) {
  Send(new PluginHostMsg_AcceleratedPluginAllocatedIOSurface(
      route_id_, width, height, surface_id));
}
