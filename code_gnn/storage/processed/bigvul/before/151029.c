void DevToolsUIBindings::SendJsonRequest(const DispatchCallback& callback,
                                         const std::string& browser_id,
                                         const std::string& url) {
  if (!android_bridge_) {
    callback.Run(nullptr);
    return;
  }
  android_bridge_->SendJsonRequest(browser_id, url,
      base::Bind(&DevToolsUIBindings::JsonReceived,
                 weak_factory_.GetWeakPtr(),
                 callback));
}
