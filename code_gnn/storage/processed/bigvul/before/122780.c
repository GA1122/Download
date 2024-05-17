int BrowserPluginGuest::embedder_routing_id() const {
  return embedder_web_contents_->GetRoutingID();
}
