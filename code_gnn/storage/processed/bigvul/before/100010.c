void WebPluginImpl::destroy() {
  SetContainer(NULL);
  MessageLoop::current()->DeleteSoon(FROM_HERE, this);
}
