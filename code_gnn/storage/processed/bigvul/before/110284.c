void Plugin::DidChangeView(const pp::View& view) {
  PLUGIN_PRINTF(("Plugin::DidChangeView (this=%p)\n",
                 static_cast<void*>(this)));

  if (!BrowserPpp::is_valid(ppapi_proxy_)) {
    view_to_replay_ = view;
  } else {
    ppapi_proxy_->ppp_instance_interface()->DidChangeView(
        pp_instance(), view.pp_resource());
  }
}
