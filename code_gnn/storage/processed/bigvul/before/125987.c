void DomOperationObserver::Observe(
    int type, const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  if (type == content::NOTIFICATION_DOM_OPERATION_RESPONSE) {
    content::Details<DomOperationNotificationDetails> dom_op_details(details);
    if (dom_op_details->automation_id == automation_id_)
      OnDomOperationCompleted(dom_op_details->json);
  } else if (type == chrome::NOTIFICATION_APP_MODAL_DIALOG_SHOWN) {
    OnModalDialogShown();
  } else {
    DCHECK_EQ(chrome::NOTIFICATION_WEB_CONTENT_SETTINGS_CHANGED, type);
    WebContents* web_contents = content::Source<WebContents>(source).ptr();
    if (web_contents) {
      TabSpecificContentSettings* tab_content_settings =
          TabSpecificContentSettings::FromWebContents(web_contents);
      if (tab_content_settings &&
          tab_content_settings->IsContentBlocked(
              CONTENT_SETTINGS_TYPE_JAVASCRIPT))
        OnJavascriptBlocked();
    }
  }
}
