PP_Var GetDocumentURL(PP_Instance instance, PP_URLComponents_Dev* components) {
  PluginInstance* plugin_instance = host_globals->GetInstance(instance);
  if (!plugin_instance)
    return PP_MakeUndefined();
  return plugin_instance->GetDocumentURL(instance, components);
}
