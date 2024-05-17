void Browser::Observe(int type,
                      const content::NotificationSource& source,
                      const content::NotificationDetails& details) {
  switch (type) {
    case content::NOTIFICATION_SSL_VISIBLE_STATE_CHANGED:
      if (chrome::GetActiveWebContents(this) &&
          &chrome::GetActiveWebContents(this)->GetController() ==
          content::Source<NavigationController>(source).ptr())
        UpdateToolbar(false);
      break;

    case chrome::NOTIFICATION_EXTENSION_UNLOADED: {
      if (window()->GetLocationBar())
        window()->GetLocationBar()->UpdatePageActions();

      if (content::Details<extensions::UnloadedExtensionInfo>(
            details)->reason != extension_misc::UNLOAD_REASON_TERMINATE) {
        const Extension* extension =
            content::Details<extensions::UnloadedExtensionInfo>(
                details)->extension;
        for (int i = tab_strip_model_->count() - 1; i >= 0; --i) {
          WebContents* tc = chrome::GetTabContentsAt(this, i)->web_contents();
          if (tc->GetURL().SchemeIs(chrome::kExtensionScheme) &&
              tc->GetURL().host() == extension->id())
            chrome::CloseWebContents(this, tc);
        }
      }
      break;
    }

    case chrome::NOTIFICATION_EXTENSION_PROCESS_TERMINATED: {
      Profile* profile = content::Source<Profile>(source).ptr();
      if (profile_->IsSameProfile(profile) && window()->GetLocationBar())
        window()->GetLocationBar()->InvalidatePageActions();
      break;
    }

    case chrome::NOTIFICATION_EXTENSION_UNINSTALLED:
    case chrome::NOTIFICATION_EXTENSION_LOADED:
      if (window() && window()->GetLocationBar())
        window()->GetLocationBar()->UpdatePageActions();
      break;

#if defined(ENABLE_THEMES)
    case chrome::NOTIFICATION_BROWSER_THEME_CHANGED:
      window()->UserChangedTheme();
      break;
#endif

    case chrome::NOTIFICATION_WEB_CONTENT_SETTINGS_CHANGED: {
      WebContents* web_contents = content::Source<WebContents>(source).ptr();
      if (web_contents == chrome::GetActiveWebContents(this)) {
        LocationBar* location_bar = window()->GetLocationBar();
        if (location_bar)
          location_bar->UpdateContentSettingsIcons();
      }
      break;
    }

    case content::NOTIFICATION_INTERSTITIAL_ATTACHED:
      UpdateBookmarkBarState(BOOKMARK_BAR_STATE_CHANGE_TAB_STATE);
      break;

    case content::NOTIFICATION_INTERSTITIAL_DETACHED:
      UpdateBookmarkBarState(BOOKMARK_BAR_STATE_CHANGE_TAB_STATE);
      break;

    default:
      NOTREACHED() << "Got a notification we didn't register for.";
  }
}
