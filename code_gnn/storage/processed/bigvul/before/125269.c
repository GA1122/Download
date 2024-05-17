WebKit::WebPlugin* RenderViewImpl::CreatePluginReplacement(
    const FilePath& file_path) {
  return GetContentClient()->renderer()->CreatePluginReplacement(
      this, file_path);
}
