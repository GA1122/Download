void EnableInternalPDFPluginForContents(int render_process_id,
                                        int render_frame_id) {
  base::FilePath pdf_plugin_path = base::FilePath::FromUTF8Unsafe(
      ChromeContentClient::kPDFPluginPath);

  content::WebPluginInfo pdf_plugin;
  if (!content::PluginService::GetInstance()->GetPluginInfoByPath(
      pdf_plugin_path, &pdf_plugin)) {
    return;
  }

  ChromePluginServiceFilter::GetInstance()->OverridePluginForFrame(
      render_process_id, render_frame_id, GURL(), pdf_plugin);
}
