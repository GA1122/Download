void ExtensionService::UpdatePluginListWithNaClModules() {
  FilePath path;
  PathService::Get(chrome::FILE_NACL_PLUGIN, &path);

  webkit::npapi::PluginList::Singleton()->UnregisterInternalPlugin(path);

  const PepperPluginInfo* pepper_info = NULL;
  std::vector<PepperPluginInfo> plugins;
  PepperPluginRegistry::ComputeList(&plugins);
  for (size_t i = 0; i < plugins.size(); ++i) {
    if (path == plugins[i].path) {
      pepper_info = &plugins[i];
      break;
    }
  }
  CHECK(pepper_info);
  webkit::npapi::WebPluginInfo info = pepper_info->ToWebPluginInfo();

  DCHECK(nacl_module_list_.size() <= 1);
  for (ExtensionService::NaClModuleInfoList::const_iterator iter =
      nacl_module_list_.begin(); iter != nacl_module_list_.end(); ++iter) {
    webkit::npapi::WebPluginMimeType mime_type_info;
    mime_type_info.mime_type = iter->mime_type;
    mime_type_info.additional_param_names.push_back(UTF8ToUTF16("nacl"));
    mime_type_info.additional_param_values.push_back(
        UTF8ToUTF16(iter->url.spec()));
    info.mime_types.push_back(mime_type_info);
  }

  webkit::npapi::PluginList::Singleton()->RefreshPlugins();
  webkit::npapi::PluginList::Singleton()->RegisterInternalPlugin(info);
}
