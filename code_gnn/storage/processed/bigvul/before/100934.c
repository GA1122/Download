ScriptableHandle* ScriptableHandle::NewDescHandle(
    DescBasedHandle* desc_handle) {
  PLUGIN_PRINTF(("ScriptableHandle::NewDescHandle (desc_handle=%p)\n",
                 static_cast<void*>(desc_handle)));
  if (desc_handle == NULL) {
    return NULL;
  }
  ScriptableHandle* scriptable_handle =
      new(std::nothrow) ScriptableHandle(desc_handle);
  if (scriptable_handle == NULL) {
    return NULL;
  }
  PLUGIN_PRINTF(("ScriptableHandle::NewDescHandle (return %p)\n",
                 static_cast<void*>(scriptable_handle)));
  return scriptable_handle;
}
