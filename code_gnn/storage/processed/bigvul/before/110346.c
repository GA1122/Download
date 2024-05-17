void Plugin::ShutdownProxy() {
  PLUGIN_PRINTF(("Plugin::ShutdownProxy (ppapi_proxy=%p)\n",
                static_cast<void*>(ppapi_proxy_)));
  if (ppapi_proxy_ != NULL) {
    ppapi_proxy_->ShutdownModule();
    delete ppapi_proxy_;
    ppapi_proxy_ = NULL;
  }
}
