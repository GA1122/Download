ChromeContentBrowserClient::GetMimeHandlerViewMimeTypes(
    content::ResourceContext* resource_context) {
  base::flat_set<std::string> mime_types;
#if BUILDFLAG(ENABLE_PLUGINS)
  auto map = PluginUtils::GetMimeTypeToExtensionIdMap(resource_context);
  for (const auto& pair : map)
    mime_types.insert(pair.first);
#endif
  return mime_types;
}
