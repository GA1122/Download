ScriptableHandle* ScriptableHandle::AddRef() {
  if (var_ == NULL) {
    var_ = new(std::nothrow) pp::VarPrivate(plugin_, this);
    CHECK(var_ != NULL);
  }
  PLUGIN_PRINTF(("ScriptableHandle::AddRef (this=%p, var=%p)\n",
                 static_cast<void*>(this), static_cast<void*>(var_)));
  return this;
}
