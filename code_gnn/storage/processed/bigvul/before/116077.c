void ExtensionSettingsHandler::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  Profile* profile = Profile::FromWebUI(web_ui());
  Profile* source_profile = NULL;
  switch (type) {
    case content::NOTIFICATION_RENDER_VIEW_HOST_DELETED:
      deleting_rvh_ = content::Source<RenderViewHost>(source).ptr();
    case content::NOTIFICATION_RENDER_VIEW_HOST_CREATED:
      source_profile = Profile::FromBrowserContext(
          content::Source<RenderViewHost>(source)->site_instance()->
          browsing_instance()->browser_context());
      if (!profile->IsSameProfile(source_profile))
        return;
      MaybeUpdateAfterNotification();
      break;
    case chrome::NOTIFICATION_BACKGROUND_CONTENTS_DELETED:
      deleting_rvh_ = content::Details<BackgroundContents>(details)->
          web_contents()->GetRenderViewHost();
    case chrome::NOTIFICATION_BACKGROUND_CONTENTS_NAVIGATED:
    case chrome::NOTIFICATION_EXTENSION_HOST_CREATED:
      source_profile = content::Source<Profile>(source).ptr();
      if (!profile->IsSameProfile(source_profile))
          return;
      MaybeUpdateAfterNotification();
      break;
    case chrome::NOTIFICATION_EXTENSION_LOADED:
    case chrome::NOTIFICATION_EXTENSION_UNLOADED:
    case chrome::NOTIFICATION_EXTENSION_UPDATE_DISABLED:
    case chrome::NOTIFICATION_EXTENSION_WARNING_CHANGED:
    case chrome::NOTIFICATION_EXTENSION_BROWSER_ACTION_VISIBILITY_CHANGED:
      MaybeUpdateAfterNotification();
      break;
    default:
      NOTREACHED();
  }
}
