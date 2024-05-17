void WebPluginDelegateProxy::OnUpdateGeometry_ACK(int ack_key) {
  DCHECK_NE(ack_key, -1);

  OldTransportDIBMap::iterator iterator = old_transport_dibs_.find(ack_key);

  DCHECK(iterator != old_transport_dibs_.end());

  ReleaseTransportDIB(iterator->second.get());

  old_transport_dibs_.erase(iterator);
}
