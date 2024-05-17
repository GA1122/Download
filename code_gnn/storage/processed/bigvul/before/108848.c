WebKit::WebPlugin* RenderViewImpl::CreatePluginReplacement(
    const FilePath& file_path) {
  return content::GetContentClient()->renderer()->CreatePluginReplacement(
      this, file_path);
}
