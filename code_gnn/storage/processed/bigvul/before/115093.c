void ShowBalloon(const Extension* extension, Profile* profile) {
  string16 message = l10n_util::GetStringFUTF16(
      extension->is_app() ?  IDS_BACKGROUND_CRASHED_APP_BALLOON_MESSAGE :
      IDS_BACKGROUND_CRASHED_EXTENSION_BALLOON_MESSAGE,
      UTF8ToUTF16(extension->name()));
  string16 content_url = DesktopNotificationService::CreateDataUrl(
      extension->GetIconURL(Extension::EXTENSION_ICON_SMALLISH,
                            ExtensionIconSet::MATCH_BIGGER),
      string16(), message, WebKit::WebTextDirectionDefault);
  Notification notification(
      extension->url(), GURL(content_url), string16(), string16(),
      new CrashNotificationDelegate(profile, extension));
  g_browser_process->notification_ui_manager()->Add(notification, profile);
}
