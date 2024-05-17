 GURL DevToolsWindow::GetDevToolsURL(Profile* profile,
                                     const GURL& base_url,
                                     bool shared_worker_frontend,
                                     bool v8_only_frontend,
                                     const std::string& remote_frontend,
                                     bool can_dock,
                                     const std::string& panel) {
   if (base_url.SchemeIs("data"))
     return base_url;
 
   std::string frontend_url(
       !remote_frontend.empty() ?
           remote_frontend :
           base_url.is_empty() ? chrome::kChromeUIDevToolsURL : base_url.spec());
   std::string url_string(
       frontend_url +
       ((frontend_url.find("?") == std::string::npos) ? "?" : "&"));
   if (shared_worker_frontend)
     url_string += "&isSharedWorker=true";
   if (v8_only_frontend)
     url_string += "&v8only=true";
   if (remote_frontend.size()) {
     url_string += "&remoteFrontend=true";
   } else {
     url_string += "&remoteBase=" + DevToolsUI::GetRemoteBaseURL().spec();
   }
   if (can_dock)
      url_string += "&can_dock=true";
    if (panel.size())
      url_string += "&panel=" + panel;
  return DevToolsUI::SanitizeFrontendURL(GURL(url_string));
//   return DevToolsUIBindings::SanitizeFrontendURL(GURL(url_string));
  }