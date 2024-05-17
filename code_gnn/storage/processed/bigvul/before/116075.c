void ExtensionSettingsHandler::MaybeUpdateAfterNotification() {
  WebContents* contents = web_ui()->web_contents();
  if (!ignore_notifications_ && contents && contents->GetRenderViewHost())
    HandleRequestExtensionsData(NULL);
  deleting_rvh_ = NULL;
}
