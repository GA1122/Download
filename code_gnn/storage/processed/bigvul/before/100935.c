ScriptableHandle* ScriptableHandle::NewPlugin(Plugin* plugin) {
  PLUGIN_PRINTF(("ScriptableHandle::NewPlugin (plugin=%p)\n",
                 static_cast<void*>(plugin)));
  if (plugin == NULL) {
    return NULL;
  }
  ScriptableHandle* scriptable_handle =
      new(std::nothrow) ScriptableHandle(plugin);
  if (scriptable_handle == NULL) {
    return NULL;
  }
  PLUGIN_PRINTF(("ScriptableHandle::NewPlugin (return %p)\n",
                 static_cast<void*>(scriptable_handle)));
  return scriptable_handle;
}
