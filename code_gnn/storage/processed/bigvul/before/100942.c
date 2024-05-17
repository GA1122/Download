bool ScriptableHandle::is_valid(const ScriptableHandle* handle) {
  PLUGIN_PRINTF(("ScriptableHandle::is_valid (handle=%p)\n",
                 static_cast<void*>(const_cast<ScriptableHandle*>(handle))));
  if (NULL == g_ValidHandles) {
    PLUGIN_PRINTF(("ScriptableHandle::is_valid (return 0)\n"));
    return false;
  }
  size_t count =
      g_ValidHandles->count(static_cast<const ScriptableHandle*>(handle));
  PLUGIN_PRINTF(("ScriptableHandle::is_valid (handle=%p, count=%"
                 NACL_PRIuS")\n",
                 static_cast<void*>(const_cast<ScriptableHandle*>(handle)),
                 count));
  return 0 != count;
}
