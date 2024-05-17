void RenderViewHostImpl::OnUpdateInspectorSetting(
    const std::string& key, const std::string& value) {
  GetContentClient()->browser()->UpdateInspectorSetting(
      this, key, value);
}
