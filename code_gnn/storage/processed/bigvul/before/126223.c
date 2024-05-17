void Browser::RegisterProtocolHandlerHelper(WebContents* web_contents,
                                            const std::string& protocol,
                                            const GURL& url,
                                            const string16& title,
                                            bool user_gesture,
                                            BrowserWindow* window) {
  TabContents* tab_contents = TabContents::FromWebContents(web_contents);
  if (!tab_contents || tab_contents->profile()->IsOffTheRecord())
    return;

  ProtocolHandler handler =
      ProtocolHandler::CreateProtocolHandler(protocol, url, title);

  ProtocolHandlerRegistry* registry =
      tab_contents->profile()->GetProtocolHandlerRegistry();
  TabSpecificContentSettings* tab_content_settings =
      TabSpecificContentSettings::FromWebContents(web_contents);

  if (registry->SilentlyHandleRegisterHandlerRequest(handler))
    return;

  if (!user_gesture && window) {
    tab_content_settings->set_pending_protocol_handler(handler);
    tab_content_settings->set_previous_protocol_handler(
        registry->GetHandlerFor(handler.protocol()));
    window->GetLocationBar()->UpdateContentSettingsIcons();
    return;
  }

  if (window) {
    tab_content_settings->ClearPendingProtocolHandler();
    window->GetLocationBar()->UpdateContentSettingsIcons();
  }

  content::RecordAction(
      UserMetricsAction("RegisterProtocolHandler.InfoBar_Shown"));
  InfoBarTabHelper* infobar_helper =
      InfoBarTabHelper::FromWebContents(web_contents);

  RegisterProtocolHandlerInfoBarDelegate* rph_delegate =
      new RegisterProtocolHandlerInfoBarDelegate(infobar_helper,
                                                 registry,
                                                 handler);

  for (size_t i = 0; i < infobar_helper->GetInfoBarCount(); i++) {
    InfoBarDelegate* delegate = infobar_helper->GetInfoBarDelegateAt(i);
    RegisterProtocolHandlerInfoBarDelegate* cast_delegate =
        delegate->AsRegisterProtocolHandlerInfoBarDelegate();
    if (cast_delegate != NULL && cast_delegate->IsReplacedBy(rph_delegate)) {
      infobar_helper->ReplaceInfoBar(cast_delegate, rph_delegate);
      rph_delegate = NULL;
      break;
    }
  }

  if (rph_delegate != NULL)
    infobar_helper->AddInfoBar(rph_delegate);
}
