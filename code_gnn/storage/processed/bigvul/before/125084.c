void ChromePluginServiceFilter::AuthorizeAllPlugins(int render_process_id) {
  AuthorizePlugin(render_process_id, FilePath());
}
