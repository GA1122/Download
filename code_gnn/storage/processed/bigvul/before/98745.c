void WebPluginDelegatePepper::StopFind() {
  find_identifier_ = -1;
  GetFindExtensions()->stopFind(instance()->npp());
}
