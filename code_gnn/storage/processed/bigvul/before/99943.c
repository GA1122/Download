NPObject *PluginInstance::GetPluginScriptableObject() {
  NPObject *value = NULL;
  NPError error = NPP_GetValue(NPPVpluginScriptableNPObject, &value);
  if (error != NPERR_NO_ERROR || value == NULL)
    return NULL;
  return value;
 }
