string16 PluginServiceImpl::GetPluginDisplayNameByPath(const FilePath& path) {
  string16 plugin_name = path.LossyDisplayName();
  webkit::WebPluginInfo info;
  if (PluginService::GetInstance()->GetPluginInfoByPath(path, &info) &&
      !info.name.empty()) {
    plugin_name = info.name;
#if defined(OS_MACOSX)
    const std::string kPluginExtension = ".plugin";
    if (EndsWith(plugin_name, ASCIIToUTF16(kPluginExtension), true))
      plugin_name.erase(plugin_name.length() - kPluginExtension.length());
#endif   
  }
  return plugin_name;
}
