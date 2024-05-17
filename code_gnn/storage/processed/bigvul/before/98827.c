WebPluginDelegateProxy::~WebPluginDelegateProxy() {
#if defined(OS_MACOSX)
  for (OldTransportDIBMap::iterator iterator = old_transport_dibs_.begin();
       iterator != old_transport_dibs_.end();
       ++iterator) {
    ReleaseTransportDIB(iterator->second.get());
  }

  ReleaseTransportDIB(transport_store_.get());
  DCHECK(!background_store_.get());
#endif
}
