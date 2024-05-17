BrowserPluginGuest* BrowserPluginGuest::Create(
    int instance_id,
    WebContentsImpl* web_contents,
    const BrowserPluginHostMsg_CreateGuest_Params& params) {
  RecordAction(UserMetricsAction("BrowserPlugin.Guest.Create"));
  if (factory_) {
    return factory_->CreateBrowserPluginGuest(instance_id,
                                              web_contents,
                                              params);
  }
  return new BrowserPluginGuest(instance_id, web_contents,params);
}
