WebPluginDelegateProxy::~WebPluginDelegateProxy() {
  if (npobject_)
    WebBindings::releaseObject(npobject_);
}
