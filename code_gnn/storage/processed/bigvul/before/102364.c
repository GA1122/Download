void ExtensionHelper::OnExtensionMessageInvoke(const std::string& extension_id,
                                               const std::string& function_name,
                                               const ListValue& args,
                                               const GURL& event_url) {
  extension_dispatcher_->v8_context_set().DispatchChromeHiddenMethod(
      extension_id, function_name, args, render_view(), event_url);
}
