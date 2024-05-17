bool Plugin::HandleInputEvent(const pp::InputEvent& event) {
  PLUGIN_PRINTF(("Plugin::HandleInputEvent (this=%p)\n",
                 static_cast<void*>(this)));
  if (!BrowserPpp::is_valid(ppapi_proxy_) ||
      ppapi_proxy_->ppp_input_event_interface() == NULL) {
    return false;   
  } else {
    bool handled = PP_ToBool(
        ppapi_proxy_->ppp_input_event_interface()->HandleInputEvent(
            pp_instance(), event.pp_resource()));
    PLUGIN_PRINTF(("Plugin::HandleInputEvent (handled=%d)\n", handled));
    return handled;
  }
}
