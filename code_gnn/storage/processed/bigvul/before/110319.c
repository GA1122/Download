  explicit MouseLockAdapter(Plugin* plugin)
    : pp::MouseLock(plugin),
      plugin_(plugin) {
    BrowserPpp* proxy = plugin_->ppapi_proxy();
    CHECK(proxy != NULL);
    ppp_mouse_lock_ = static_cast<const PPP_MouseLock*>(
        proxy->GetPluginInterface(PPP_MOUSELOCK_INTERFACE));
  }
