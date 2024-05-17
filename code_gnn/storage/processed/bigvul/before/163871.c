GURL DevToolsWindow::GetDevToolsURL(Profile* profile,
                                    FrontendType frontend_type,
                                    const std::string& frontend_url,
                                    bool can_dock,
                                    const std::string& panel) {
  std::string url(!frontend_url.empty() ? frontend_url
                                        : chrome::kChromeUIDevToolsURL);
  std::string url_string(url +
                         ((url.find("?") == std::string::npos) ? "?" : "&"));
  switch (frontend_type) {
    case kFrontendRemote:
      url_string += "&remoteFrontend=true";
      break;
    case kFrontendWorker:
      url_string += "&isSharedWorker=true";
      break;
    case kFrontendNode:
      url_string += "&nodeFrontend=true";
    case kFrontendV8:
      url_string += "&v8only=true";
      break;
    case kFrontendDefault:
    default:
      break;
  }

  if (frontend_url.empty())
    url_string += "&remoteBase=" + DevToolsUI::GetRemoteBaseURL().spec();
  if (can_dock)
    url_string += "&can_dock=true";
  if (panel.size())
    url_string += "&panel=" + panel;
  return DevToolsUIBindings::SanitizeFrontendURL(GURL(url_string));
}
