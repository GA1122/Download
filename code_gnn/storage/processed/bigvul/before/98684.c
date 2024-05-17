WebPluginResourceClient* WebPluginDelegatePepper::CreateSeekableResourceClient(
    unsigned long resource_id, int range_request_id) {
  return instance()->GetRangeRequest(range_request_id);
}
