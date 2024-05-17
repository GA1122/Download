scoped_refptr<DevToolsAgentHost> DevToolsAgentHost::GetForId(
    const std::string& id) {
  if (!g_devtools_instances.IsCreated())
    return nullptr;
  DevToolsMap::iterator it = g_devtools_instances.Get().find(id);
  if (it == g_devtools_instances.Get().end())
    return nullptr;
  return it->second;
}
