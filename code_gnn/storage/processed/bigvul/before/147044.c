void WebLocalFrameImpl::Reload(WebFrameLoadType load_type) {
  ReloadWithOverrideURL(NullURL(), load_type);
}
