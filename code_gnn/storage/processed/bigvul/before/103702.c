void DevToolsAgent::clearBrowserCookies() {
  Send(new DevToolsHostMsg_ClearBrowserCookies(routing_id()));
}
