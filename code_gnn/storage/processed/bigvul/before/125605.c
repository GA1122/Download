void RenderViewHostImpl::ExecutePluginActionAtLocation(
  const gfx::Point& location, const WebKit::WebPluginAction& action) {
  Send(new ViewMsg_PluginActionAt(GetRoutingID(), location, action));
}
