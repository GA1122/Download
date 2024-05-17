void WebPluginDelegatePepper::SelectFindResult(bool forward) {
  GetFindExtensions()->selectFindResult(instance()->npp(), forward);
}
