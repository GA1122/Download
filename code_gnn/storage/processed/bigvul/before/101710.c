void Browser::Observe(int type,
                      const NotificationSource& source,
                      const NotificationDetails& details) {
  switch (type) {
    case content::NOTIFICATION_TAB_CONTENTS_DISCONNECTED:
      if (is_attempting_to_close_browser_) {
        ClearUnloadState(Source<TabContents>(source).ptr(), false);
      }
      break;

    case content::NOTIFICATION_SSL_VISIBLE_STATE_CHANGED:
      if (GetSelectedTabContents() &&
          &GetSelectedTabContents()->controller() ==
          Source<NavigationController>(source).ptr())
        UpdateToolbar(false);
      break;

    case chrome::NOTIFICATION_EXTENSION_UPDATE_DISABLED: {
      Profile* profile = Source<Profile>(source).ptr();
      if (profile_->IsSameProfile(profile)) {
        ExtensionService* service = profile->GetExtensionService();
        DCHECK(service);
        const Extension* extension = Details<const Extension>(details).ptr();
        if (service->extension_prefs()->DidExtensionEscalatePermissions(
                extension->id()))
          ShowExtensionDisabledUI(service, profile_, extension);
      }
      break;
    }

    case chrome::NOTIFICATION_EXTENSION_UNLOADED: {
      if (window()->GetLocationBar())
        window()->GetLocationBar()->UpdatePageActions();

      const Extension* extension =
          Details<UnloadedExtensionInfo>(details)->extension;
      TabStripModel* model = tab_handler_->GetTabStripModel();
      for (int i = model->count() - 1; i >= 0; --i) {
        TabContents* tc = model->GetTabContentsAt(i)->tab_contents();
        if (tc->GetURL().SchemeIs(chrome::kExtensionScheme) &&
            tc->GetURL().host() == extension->id()) {
          CloseTabContents(tc);
        }
      }

      break;
    }

    case chrome::NOTIFICATION_EXTENSION_PROCESS_TERMINATED: {
      Profile* profile = Source<Profile>(source).ptr();
      if (profile_->IsSameProfile(profile) && window()->GetLocationBar())
        window()->GetLocationBar()->InvalidatePageActions();
      break;
    }

    case chrome::NOTIFICATION_EXTENSION_UNINSTALLED:
    case chrome::NOTIFICATION_EXTENSION_LOADED:
      if (window() && window()->GetLocationBar())
        window()->GetLocationBar()->UpdatePageActions();
      break;

    case chrome::NOTIFICATION_BROWSER_THEME_CHANGED:
      window()->UserChangedTheme();
      break;

    case chrome::NOTIFICATION_EXTENSION_READY_FOR_INSTALL: {
      if (BrowserList::FindTabbedBrowser(profile(), true) != this)
        break;

      GURL download_url = *(Details<GURL>(details).ptr());
      if (ExtensionService::IsDownloadFromMiniGallery(download_url))
        window()->ShowThemeInstallBubble();
      break;
    }

    case chrome::NOTIFICATION_PREF_CHANGED: {
      const std::string& pref_name = *Details<std::string>(details).ptr();
      if (pref_name == prefs::kUseVerticalTabs) {
        UseVerticalTabsChanged();
      } else if (pref_name == prefs::kUseCompactNavigationBar) {
        UseCompactNavigationBarChanged();
      } else if (pref_name == prefs::kPrintingEnabled) {
        UpdatePrintingState(GetContentRestrictionsForSelectedTab());
      } else if (pref_name == prefs::kInstantEnabled ||
                 pref_name == prefs::kMetricsReportingEnabled ||
                 pref_name == prefs::kSearchSuggestEnabled) {
        if (!InstantController::IsEnabled(profile())) {
          if (instant()) {
            instant()->DestroyPreviewContents();
            instant_.reset();
            instant_unload_handler_.reset();
          }
        } else {
          CreateInstantIfNecessary();
        }
      } else if (pref_name == prefs::kIncognitoModeAvailability) {
        IncognitoModePrefs::Availability available =
            IncognitoModePrefs::GetAvailability(profile_->GetPrefs());
        command_updater_.UpdateCommandEnabled(
            IDC_NEW_WINDOW,
            available != IncognitoModePrefs::FORCED);
        command_updater_.UpdateCommandEnabled(
            IDC_NEW_INCOGNITO_WINDOW,
            available != IncognitoModePrefs::DISABLED);
      } else if (pref_name == prefs::kDevToolsDisabled) {
        UpdateCommandsForDevTools();
        if (profile_->GetPrefs()->GetBoolean(prefs::kDevToolsDisabled))
          g_browser_process->devtools_manager()->CloseAllClientHosts();
      } else if (pref_name == prefs::kEditBookmarksEnabled) {
        UpdateCommandsForBookmarkEditing();
      } else if (pref_name == prefs::kEnableBookmarkBar) {
        UpdateCommandsForBookmarkBar();
      } else if (pref_name == prefs::kHomePage) {
        PrefService* pref_service = Source<PrefService>(source).ptr();
        MarkHomePageAsChanged(pref_service);
      } else if (pref_name == prefs::kAllowFileSelectionDialogs) {
        UpdateSaveAsState(GetContentRestrictionsForSelectedTab());
        UpdateOpenFileState();
      } else {
        NOTREACHED();
      }
      break;
    }

    case chrome::NOTIFICATION_TAB_CONTENT_SETTINGS_CHANGED: {
      TabContents* tab_contents = Source<TabContents>(source).ptr();
      if (tab_contents == GetSelectedTabContents()) {
        LocationBar* location_bar = window()->GetLocationBar();
        if (location_bar)
          location_bar->UpdateContentSettingsIcons();
      }
      break;
    }

    case content::NOTIFICATION_INTERSTITIAL_ATTACHED:
      UpdateBookmarkBarState(BOOKMARK_BAR_STATE_CHANGE_TAB_STATE);
      break;

    case chrome::NOTIFICATION_BOOKMARK_BAR_VISIBILITY_PREF_CHANGED:
      if (profile_->IsSameProfile(Source<Profile>(source).ptr()))
        UpdateBookmarkBarState(BOOKMARK_BAR_STATE_CHANGE_PREF_CHANGE);
      break;

    default:
      NOTREACHED() << "Got a notification we didn't register for.";
  }
}
