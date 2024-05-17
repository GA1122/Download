void Browser::RegisterProtocolHandler(WebContents* web_contents,
                                      const std::string& protocol,
                                      const GURL& url,
                                      bool user_gesture) {
  content::BrowserContext* context = web_contents->GetBrowserContext();
  if (context->IsOffTheRecord())
    return;

  if (vr::VrTabHelper::IsInVr(web_contents))
    return;

  ProtocolHandler handler =
      ProtocolHandler::CreateProtocolHandler(protocol, url);

  ProtocolHandlerRegistry* registry =
      ProtocolHandlerRegistryFactory::GetForBrowserContext(context);
  if (registry->SilentlyHandleRegisterHandlerRequest(handler))
    return;

  TabSpecificContentSettings* tab_content_settings =
      TabSpecificContentSettings::FromWebContents(web_contents);
  if (!user_gesture && window_) {
    tab_content_settings->set_pending_protocol_handler(handler);
    tab_content_settings->set_previous_protocol_handler(
        registry->GetHandlerFor(handler.protocol()));
    window_->GetLocationBar()->UpdateContentSettingsIcons();
    return;
  }

  if (window_) {
    tab_content_settings->ClearPendingProtocolHandler();
    window_->GetLocationBar()->UpdateContentSettingsIcons();
  }

  PermissionRequestManager* permission_request_manager =
      PermissionRequestManager::FromWebContents(web_contents);
  if (permission_request_manager) {
    permission_request_manager->AddRequest(
        new RegisterProtocolHandlerPermissionRequest(registry, handler,
                                                     url, user_gesture));
  }
}
