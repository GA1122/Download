void ExtensionHelper::DidDownloadApplicationDefinition(
    const WebKit::WebURLResponse& response,
    const std::string& data) {
  scoped_ptr<WebApplicationInfo> app_info(
      pending_app_info_.release());

  JSONStringValueSerializer serializer(data);
  int error_code = 0;
  std::string error_message;
  scoped_ptr<Value> result(serializer.Deserialize(&error_code, &error_message));
  if (!result.get()) {
    AddErrorToRootConsole(UTF8ToUTF16(error_message));
    return;
  }

  string16 error_message_16;
  if (!web_apps::ParseWebAppFromDefinitionFile(result.get(), app_info.get(),
                                               &error_message_16)) {
    AddErrorToRootConsole(error_message_16);
    return;
  }

  if (!app_info->icons.empty()) {
    pending_app_info_.reset(app_info.release());
    pending_app_icon_requests_ =
        static_cast<int>(pending_app_info_->icons.size());
    for (size_t i = 0; i < pending_app_info_->icons.size(); ++i) {
      app_icon_fetchers_.push_back(linked_ptr<ImageResourceFetcher>(
          new ImageResourceFetcher(
              pending_app_info_->icons[i].url,
              render_view()->GetWebView()->mainFrame(),
              static_cast<int>(i),
              pending_app_info_->icons[i].width,
              WebURLRequest::TargetIsFavicon,
              base::Bind(
                  &ExtensionHelper::DidDownloadApplicationIcon,
                  base::Unretained(this)))));
    }
  } else {
    Send(new ExtensionHostMsg_InstallApplication(routing_id(), *app_info));
  }
}
