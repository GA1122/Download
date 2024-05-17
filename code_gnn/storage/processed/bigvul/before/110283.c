void Plugin::DidChangeFocus(bool has_focus) {
  PLUGIN_PRINTF(("Plugin::DidChangeFocus (this=%p)\n",
                 static_cast<void*>(this)));
  if (BrowserPpp::is_valid(ppapi_proxy_)) {
    ppapi_proxy_->ppp_instance_interface()->DidChangeFocus(
        pp_instance(), PP_FromBool(has_focus));
  }
}
