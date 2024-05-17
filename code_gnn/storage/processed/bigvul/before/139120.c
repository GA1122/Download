void RenderProcessHostImpl::AddRoute(int32_t routing_id,
                                     IPC::Listener* listener) {
  CHECK(!listeners_.Lookup(routing_id)) << "Found Routing ID Conflict: "
                                        << routing_id;
  listeners_.AddWithID(listener, routing_id);
}
